#include <iostream>
#include <fstream>
#include <cstring>
#include <array>
#include <algorithm>
#include <list>
#include <iterator>
#include <string>
#include <bits/stdc++.h>

using namespace std;

struct PIndexEmp{
    char ID[13];
    int RRN;
    bool operator<(const PIndexEmp &e) const {
		return strcmp(ID, e.ID) < 0;
	}
};

struct PIndexDep{
    char ID[30];
    int RRN;
    bool operator<(const PIndexDep &d) const {
		return strcmp(ID, d.ID) < 0;
	}
};

struct SIndexEmp{
    char Dept_ID[30];
    int RRN;
    bool operator<(const SIndexEmp &e) const {
        return strcmp(Dept_ID, e.Dept_ID) < 0;
			}
};

struct SIndexDep{
    char Name[30];
    int RRN;
    bool operator<(const SIndexDep &d) const {
		return strcmp(Name, d.Name) < 0;
	}
};

struct LEmp{
    int RRN;
    char ID[13];
    int next;
    int back1;
};

struct LDep{
    int RRN;
    char ID[30];
    int next;
    int back1;
};

void WritePrimaryIndexEmp(fstream& stream,int r,PIndexEmp e){
    stream.close();
    stream.open("ep.txt",ios::out|ios::in);
    stream.seekg(0,ios::end);
    PIndexEmp* emp=new PIndexEmp[r+2];
    if(stream.tellg()==0){
        stream.clear();
        stream.seekp(0,ios::beg);
        stream.write((char*)&e,sizeof(e));
    }
    else{
        int end=stream.tellg();
        stream.seekg(0,ios::beg);
        int i=0;
        while(stream.tellg()!=end){
            stream.read((char*)&emp[i],sizeof(emp[i]));
            i++;
        }
        emp[i]=e;
        i++;
        sort(emp,emp+i);
        stream.clear();
        stream.seekp(0,ios::beg);
        for(int j=0;j<i;j++){
            stream.write((char*)&emp[j],sizeof(emp[j]));
        }
    }
}

void WritePrimaryIndexDep(fstream& stream,int r,PIndexDep d){
    stream.seekg(0,ios::end);
    PIndexDep* dep=new PIndexDep[r];
    if(stream.tellg()==0){
        stream.seekp(0,ios::beg);
        stream.write((char*)&d,sizeof(d));
    }
    else{
        int end=stream.tellg();
        stream.seekg(0,ios::beg);
        int i=0;
        while(stream.tellg()!=end){
            stream.read((char*)&dep[i],sizeof(dep[i]));
            i++;
        }
        dep[i]=d;
        i++;
        sort(dep,dep+i);
        stream.clear();
        stream.seekp(0,ios::beg);
        for(int j=0;j<i;j++){
            stream.write((char*)&dep[j],sizeof(dep[j]));
        }
    }
}

PIndexEmp* ReadPIndE(fstream& stream,int r){
    PIndexEmp* emp=new PIndexEmp[r];
    stream.seekg(0,ios::end);
    int end=stream.tellg();
    stream.seekg(0,ios::beg);
    PIndexEmp em;
    int i=0;
    while(stream.tellg()!=end){
        stream.read((char*)&emp[i],sizeof(emp[i]));
    }
    return emp;
}

PIndexDep* ReadPIndD(fstream& stream,int r){
    PIndexDep* dep=new PIndexDep[r];
    stream.seekg(0,ios::end);
    int end=stream.tellg();
    stream.seekg(0,ios::beg);
    PIndexDep de;
    int i=0;
    while(stream.tellg()!=end){
        stream.read((char*)&dep[i],sizeof(dep[i]));
    }
    return dep;
}

void WriteSecondaeryIndexEmp(fstream& stream,int r,SIndexEmp e,char* ID){
    stream.close();
    stream.open("es.txt",ios::out|ios::in);
    fstream file1("el.txt",ios::out|ios::in);
    stream.seekg(0,ios::end);
    SIndexEmp* emp=new SIndexEmp[r+2];
    if(stream.tellg()==0){
        stream.clear();
        stream.seekp(0,ios::beg);
        e.RRN=file1.tellg();
        stream.write((char*)&e,sizeof(e));
        LEmp le;
        le.RRN=e.RRN;
        strcpy(le.ID,ID);
        le.next=-1;
        le.back1=-1;
        file1.write((char*)&le,sizeof(le));
    }
    else{
        int end=stream.tellg();
        stream.seekg(0,ios::beg);
        int i=0;
        LEmp le;
        while(stream.tellg()!=end){
            stream.read((char*)&emp[i],sizeof(emp[i]));
            i++;
        }
        bool state=false;
        for(int j=0;j<i;j++){
            if(strcmp(emp[j].Dept_ID,e.Dept_ID)==0){
                    file1.seekg(emp[j].RRN,ios::beg);
                    file1.read((char*)&le,sizeof(le));
                    if(strcmp(ID,le.ID)<0){
                            LEmp LE;
                            LE.next=emp[j].RRN;
                            file1.seekp(0,ios::end);
                            le.back1=file1.tellp();
                            emp[j].RRN=file1.tellp();
                            file1.seekp(le.RRN,ios::beg);
                            file1.write((char*)&le,sizeof(le));
                            strcpy(LE.ID,ID);
                            LE.back1=-1;
                            file1.seekp(0,ios::end);
                            file1.write((char*)&LE,sizeof(LE));
                    }
                    else if(strcmp(ID,le.ID)>0){
                        do{
                            file1.seekg(le.next,ios::beg);
                        }
                        while(strcmp(ID,le.ID)>0);
                        LEmp LE;
                        LEmp Le;
                        int back=le.back1;
                        file1.seekg(Le.back1,ios::beg);
                        file1.read((char*)&Le,sizeof(Le));
                        int current=Le.next;
                        strcpy(LE.ID,ID);
                        LE.next=current;
                        LE.back1=back;
                        file1.seekp(0,ios::end);
                        le.back1=file1.tellp();
                        Le.next=file1.tellp();
                        file1.write((char*)&LE,sizeof(LE));
                        file1.seekp(current,ios::beg);
                        file1.write((char*)&le,sizeof(le));
                        file1.seekp(back,ios::beg);
                        file1.write((char*)&Le,sizeof(Le));
                    }
                    state=true;
                    break;
            }
            state=false;
        }
        if(!state){
            emp[i]=e;
            i++;
            sort(emp,emp+i);
            file1.seekp(0,ios::end);
            LEmp LE;
            strcpy(LE.ID,ID);
            LE.next=-1;
            LE.back1=-1;
            emp[i].RRN=file1.tellp();
            file1.write((char*)&LE,sizeof(LE));
        }
        stream.clear();
        stream.seekp(0,ios::beg);
        for(int j=0;j<i;j++){
            stream.write((char*)&emp[j],sizeof(emp[j]));
        }
        file1.close();
    }
}

void WriteSecondaeryIndexDep(fstream& stream,int r,SIndexDep d,char* ID){
    stream.close();
    stream.open("ds.txt",ios::out|ios::in);
    fstream file1("dl.txt",ios::out|ios::in);
    stream.seekg(0,ios::end);
    SIndexDep* dep=new SIndexDep[r+2];
    if(stream.tellg()==0){
        stream.clear();
        stream.seekp(0,ios::beg);
        d.RRN=file1.tellp();
        stream.write((char*)&d,sizeof(d));
        LDep ld;
        ld.RRN=d.RRN;
        strcpy(ld.ID,ID);
        ld.next=-1;
        ld.back1=-1;
        file1.write((char*)&ld,sizeof(ld));

    }
    else{
        int end=stream.tellg();
        stream.seekg(0,ios::beg);
        int i=0;
        LDep ld;
        while(stream.tellg()!=end){
            stream.read((char*)&dep[i],sizeof(dep[i]));
            i++;
        }
        bool state;
        for(int j=0;j<i;j++){
            if(strcmp(dep[j].Name,d.Name)==0){
                    file1.seekg(dep[j].RRN,ios::beg);
                    file1.read((char*)&ld,sizeof(ld));
                    if(strcmp(ID,ld.ID)<0){
                            LDep LD;
                            LD.next=dep[j].RRN;
                            file1.seekp(0,ios::end);
                            ld.back1=file1.tellp();
                            dep[j].RRN=file1.tellp();
                            file1.seekp(ld.RRN,ios::beg);
                            file1.write((char*)&ld,sizeof(ld));
                            strcpy(LD.ID,ID);
                            LD.back1=-1;
                            file1.seekp(0,ios::end);
                            file1.write((char*)&LD,sizeof(LD));
                    }
                    else if(strcmp(ID,ld.ID)>0){
                        do{
                            file1.seekg(ld.next,ios::beg);
                        }
                        while(strcmp(ID,ld.ID)>0);
                        LDep LD;
                        LDep Ld;
                        int back=ld.back1;
                        file1.seekg(Ld.back1,ios::beg);
                        file1.read((char*)&Ld,sizeof(Ld));
                        int current=Ld.next;
                        strcpy(LD.ID,ID);
                        LD.next=current;
                        LD.back1=back;
                        file1.seekp(0,ios::end);
                        ld.back1=file1.tellp();
                        Ld.next=file1.tellp();
                        file1.write((char*)&LD,sizeof(LD));
                        file1.seekp(current,ios::beg);
                        file1.write((char*)&ld,sizeof(ld));
                        file1.seekp(back,ios::beg);
                        file1.write((char*)&Ld,sizeof(Ld));
                    }
                    state=true;
                    break;
            }
            state=false;
        }
        if(!state){
            dep[i]=d;
            i++;
            sort(dep,dep+i);
            file1.seekp(0,ios::end);
            LDep LD;
            strcpy(LD.ID,ID);
            LD.next=-1;
            LD.back1=-1;
            dep[i].RRN=file1.tellp();
            file1.write((char*)&LD,sizeof(LD));
        }
        stream.clear();
        stream.seekp(0,ios::beg);
        for(int j=0;j<i;j++){
            cout<<1<<endl;
            stream.write((char*)&dep[j],sizeof(dep[j]));
        }
        file1.close();
    }
}

SIndexEmp* ReadSIndE(fstream& stream,int r){
    SIndexEmp* emp=new SIndexEmp[r];
    stream.seekg(0,ios::beg);
    SIndexEmp em;
    int i=0;
    for(i=0;i<r;i++){
        stream.read((char*)&emp[i],sizeof(emp[i]));
    }
    return emp;
}

SIndexDep* ReadSIndD(fstream& stream,int r){
    SIndexDep* dep=new SIndexDep[r];
    stream.seekg(0,ios::beg);
    SIndexDep de;
    int i=0;
    for(i=0;i<r;i++){
        stream.read((char*)&dep[i],sizeof(dep[i]));
    }
    return dep;
}

class Employee{
private:
    char Employee_ID[13];
    char Dept_ID[30];
    char Employee_Name[50];
    char Employee_Position[50];
    int RRN;
public:
    Employee(){
        strcpy(Employee_ID,"             ");
        strcpy(Dept_ID,"                              ");
        strcpy(Employee_Name,"                                                  ");
        strcpy(Employee_Position,"                                                  ");
    }

    Employee(char* ID,char* D_ID,char* Name,char* Position){
        strcpy(Employee_ID,ID);
        strcpy(Dept_ID,D_ID);
        strcpy(Employee_Name,Name);
        strcpy(Employee_Position,Position);
    }

    void SetRRN(int rrn){
        RRN=rrn;
    }

    void Set(char* ID,char* D_ID,char* Name,char* Position){
        strcpy(Employee_ID,ID);
        strcpy(Dept_ID,D_ID);
        strcpy(Employee_Name,Name);
        strcpy(Employee_Position,Position);
    }

    void Get(){
        cout<<"ID: "<<Employee_ID<<endl;
        cout<<"Department ID: "<<Dept_ID<<endl;
        cout<<"Name: "<<Employee_Name<<endl;
        cout<<"Position: "<<Employee_Position<<endl;
    }

    void GetName(){
        cout<<"Name: "<<Employee_Name<<endl;
    }

    friend istream & operator >> (istream& Input,Employee &e){
        char id[13];
        char did[30];
        char n[50];
        char p[50];
        cout<<"Enter Your ID: ";
        Input>>id;
        cout<<"Enter Your Department ID: ";
        Input>>did;
        cout<<"Enter Your Name: ";
        Input>>n;
        cout<<"Enter Your Position: ";
        Input>>p;
        e.Set(id,did,n,p);
        return Input;
    }

    int WriteEmployee(fstream& stream){
        fstream file1("ep.txt",ios::out|ios::in);
        fstream file2("es.txt",ios::out|ios::in);
        RRN=-1;
        int firstDeleted=-1,nextDeleted=-1;
        PIndexEmp e;
        SIndexEmp e1;
        stream.seekg(0,ios::beg);
        stream.read((char*)&firstDeleted,sizeof(int));
        RRN=firstDeleted;
        if(RRN==-1){
            stream.seekp(0,ios::end);
            RRN=stream.tellp();
        }
        else{
            stream.seekg(RRN,ios::beg);
            char c;
            stream.get(c);
            if(c!='*'){
                return -1;
            }
            stream.read((char*)&nextDeleted,sizeof(int));
            stream.seekp(0,ios::beg);
            stream.write((char*)&nextDeleted,sizeof(int));
            stream.seekp(RRN,ios::beg);
        }
        e.RRN=RRN;
        strcpy(e.ID,Employee_ID);
        strcpy(e1.Dept_ID,Dept_ID);
        Write(stream);
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        WritePrimaryIndexEmp(file1,r,e);
        WriteSecondaeryIndexEmp(file2,r,e1,Employee_ID);
        file1.close();
        file2.close();
        return RRN;
    }

    void Write(fstream& stream){
        int nd=-1;
        stream.put(' ');
        stream.write((char*)&nd,sizeof(int));
        stream.write(Employee_ID,strlen(Employee_ID));
        stream.put('|');
        stream.write(Dept_ID,strlen(Dept_ID));
        stream.put('|');
        stream.write(Employee_Name,strlen(Employee_Name));
        stream.put('|');
        stream.write(Employee_Position,strlen(Employee_Position));
        stream.put('|');
        stream.put('$');
    }

    Employee GetEmployee(int rrn,fstream& stream){
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        Employee e;
        e.SetRRN(rrn);
        stream.seekg(rrn,ios::beg);
        e.read(stream);
        return e;
    }

    void read(fstream& stream){
        int nextDeleted=-1;
        char c;
        stream.get(c);
        if(c=='*'){
            cout<<"Deleted Record"<<endl;
            return;
        }
        stream.read((char*)&nextDeleted,sizeof(int));
        stream.getline(Employee_ID,13,'|');
        stream.getline(Dept_ID,30,'|');
        stream.getline(Employee_Name,50,'|');
        stream.getline(Employee_Position,50,'|');
    }

    bool DeleteEmployee(int rrn,fstream& stream){
        int firstDeleted=-1;
        int maxbyteoffset=0;
        char c;
        stream.seekg(0,ios::end);
        maxbyteoffset=stream.tellg();
        if(rrn>=maxbyteoffset){
            return false;
        }
        stream.seekg(0,ios::beg);
        stream.read((char*)&firstDeleted,sizeof(int));
        stream.seekg(rrn,ios::beg);
        stream.get(c);
        if(c=='*'){
            return false;
        }
        stream.seekp(rrn,ios::beg);
        stream.put('*');
        stream.write((char*)&firstDeleted,sizeof(int));
        stream.seekp(0,ios::beg);
        stream.write((char*)&rrn,sizeof(int));
        return true;
    }

    void DeleteEmployeeByID(char* ID,fstream& stream){
        fstream file("ep.txt",ios::in|ios::out);
        PIndexEmp* emp;
        int r=numofRecords(stream);
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        emp=ReadPIndE(file,r);
        int rrn=Binarysearch(emp,ID,0,r-1,r);
        if(DeleteEmployee(rrn,stream)){}
        file.close();
    }

    int numofRecords(fstream& stream){
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        int count=0;
        char c[156];
        stream.seekg(0,ios::beg);
        while(stream.getline(c,156,'$')){
            count++;
        }
        return count;
    }

    void searchByID(char* ID,fstream& stream){
        fstream file1("ep.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        PIndexEmp* emp;
        emp=ReadPIndE(file1,r);
        int rrn=GetRecordByID(emp,ID,r);
        Employee e=GetEmployee(rrn,stream);
        e.Get();
        file1.close();
    }

    void searchByIDName(char* ID,fstream& stream){
        fstream file1("ep.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        PIndexEmp* emp;
        emp=ReadPIndE(file1,r);
        int rrn=GetRecordByID(emp,ID,r);
        Employee e=GetEmployee(rrn,stream);
        e.GetName();
        file1.close();
    }

    int GetRecordByID(PIndexEmp* emp,char* ID,int r){
        int low=0,high=r-1,mid;
        return Binarysearch(emp,ID,low,high,r);
    }

    int Binarysearch(PIndexEmp* emp,char* ID,int b,int e,int r){
            int mid;
            while(b<=e){
                mid = (b + e) / 2;
                if(strcmp(ID,emp[mid].ID)==0){
                    return emp[mid].RRN;
                    }
                else if (strcmp(ID , emp[mid].ID)<0)
                    e = mid - 1;
                else if (strcmp(ID , emp[mid].ID)>0)
                    b = mid + 1;
                }
            return -1;
    }

    void searchByDeptId(char* ID,fstream& stream){
        fstream file1("es.txt",ios::out|ios::in);
        fstream file2("el.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        stream.close();
        stream.open("e.txt",ios::out|ios::in);
        SIndexEmp* emp;
        emp=ReadSIndE(file1,r);
        int rrn=GetRecordsByDeptID(emp,ID,r);
        LEmp l;
        file2.seekg(rrn,ios::beg);
        do{
            file2.read((char*)&l,sizeof(l));
            searchByID(l.ID,stream);
            file2.seekg(l.next,ios::beg);
        }
        while(l.next!=-1);
        file1.close();
        file2.close();
    }

    int GetRecordsByDeptID(SIndexEmp* emp,char* ID,int r){
        int low=0,high=r-1,mid;
        return BinarysearchD(emp,ID,low,high,r);
    }

    int BinarysearchD(SIndexEmp* emp,char* ID,int b,int e,int r){
            int mid;
            while(b<=e){
                mid = (b + e) / 2;
                if(strcmp(ID,emp[mid].Dept_ID)==0){
                    return emp[mid].RRN;
                    break;
                    }
                if (strcmp(ID , emp[mid].Dept_ID)<0)
                    e = mid - 1;
                if (strcmp(ID , emp[mid].Dept_ID)>0)
                    b = mid + 1;
                }
            return -1;
    }
};

class Department{
private:
    char Dept_ID[30];
    char Dept_Name[50];
    char Dept_Manger[50];
    int RRN;
public:
    Department(){
        strcpy(Dept_ID,"                              ");
        strcpy(Dept_Name,"                                                  ");
        strcpy(Dept_Manger,"                                                  ");
    }

    Department(char* ID,char* Name,char* Manger){
        strcpy(Dept_ID,ID);
        strcpy(Dept_Name,Name);
        strcpy(Dept_Manger,Manger);
    }

    void Set(char* ID,char* Name,char* Manger){
         strcpy(Dept_ID,ID);
        strcpy(Dept_Name,Name);
        strcpy(Dept_Manger,Manger);
    }

    void Get(){
        cout<<"ID: "<<Dept_ID<<endl;
        cout<<"Department Name: "<<Dept_Name<<endl;
        cout<<"Department Manager: "<<Dept_Manger<<endl;
    }


    void SetRRN(int rrn){
        RRN=rrn;
    }

    friend istream & operator >> (istream& Input,Department &d){
        char id[30];
        char n[50];
        char m[50];
        cout<<"Enter Department ID: ";
        Input>>id;
        cout<<"Enter Department Name: ";
        Input>>n;
        cout<<"Enter Department Manger: ";
        Input>>m;
        d.Set(id,n,m);
        return Input;
    }

    int WriteDeparment(fstream& stream){
        fstream file1("dp.txt",ios::out|ios::in);
        fstream file2("ds.txt",ios::out|ios::in);
        RRN=-1;
        PIndexDep d;
        SIndexDep d1;
        int firstDeleted=-1,nextDeleted=-1;
        stream.seekg(0,ios::beg);
        stream.read((char*)&firstDeleted,sizeof(int));
        RRN=firstDeleted;
        if(RRN==-1){
            stream.seekp(0,ios::end);
            RRN=stream.tellp();
        }
        else{
            stream.seekg(RRN,ios::beg);
            char c;
            stream.get(c);
            if(c!='*'){
                return -1;
            }
            stream.read((char*)&nextDeleted,sizeof(int));
            stream.seekp(0,ios::beg);
            stream.write((char*)&nextDeleted,sizeof(int));
            stream.seekp(RRN,ios::beg);
        }
        d.RRN=RRN;
        strcpy(d.ID,Dept_ID);
        strcpy(d1.Name,Dept_Name);
        Write(stream);
        stream.close();
        stream.open("d.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        WritePrimaryIndexDep(file1,r,d);
        WriteSecondaeryIndexDep(file2,r,d1,Dept_ID);
        file1.close();
        file2.close();
        return RRN;
    }

    void Write(fstream& stream){
        int nd=-1;
        stream.put(' ');
        stream.write((char*)&nd,sizeof(int));
        stream.write(Dept_ID,strlen(Dept_ID));
        stream.put('|');
        stream.write(Dept_Name,strlen(Dept_Name));
        stream.put('|');
        stream.write(Dept_Manger,strlen(Dept_Manger));
        stream.put('|');
        stream.put('$');
    }

    Department GetDepartment(int rrn,fstream& stream){
        stream.close();
        stream.open("d.txt",ios::out|ios::in);
        Department d;
        d.SetRRN(rrn);
        stream.seekg(rrn,ios::beg);
        d.read(stream);
        return d;
    }

    void read(fstream& stream){
        int nextDeleted=-1;
        char c;
        stream.get(c);
        if(c=='*'){
            cout<<"Deleted Record"<<endl;
            return;
        }
        stream.read((char*)&nextDeleted,sizeof(int));
        stream.getline(Dept_ID,30,'|');
        stream.getline(Dept_Name,50,'|');
        stream.getline(Dept_Manger,50,'|');
    }

    bool DeleteDepartment(int rrn,fstream& stream){
        int firstDeleted=-1;
        int maxbyteoffset=0;
        char c;
        stream.seekg(0,ios::end);
        maxbyteoffset=stream.tellg();
        if(rrn>=maxbyteoffset){
            return false;
        }
        stream.seekg(0,ios::beg);
        stream.read((char*)&firstDeleted,sizeof(int));
        stream.seekg(rrn,ios::beg);
        stream.get(c);
        if(c=='*'){
            return false;
        }
        stream.seekp(rrn,ios::beg);
        stream.put('*');
        stream.write((char*)&firstDeleted,sizeof(int));
        stream.seekp(0,ios::beg);
        stream.write((char*)&rrn,sizeof(int));
        return true;
    }

    void DeleteDepartmentByID(char* ID,fstream& stream){
        fstream file("dp.txt",ios::in|ios::out);
        PIndexDep* dep;
        int r=numofRecords(stream);
        stream.close();
        stream.open("d.txt",ios::out|ios::in);
        dep=ReadPIndD(file,r);
        int rrn=Binarysearch(dep,ID,0,r-1,r);
        if(DeleteDepartment(rrn,stream));
        file.close();
    }

    int numofRecords(fstream& stream){
        stream.close();
        stream.open("d.txt",ios::out|ios::in);
        int count=0;
        char c[148];
        stream.seekg(0,ios::beg);
        while(stream.getline(c,148,'$')){
            count++;
        }
        return count;
    }

    void searchByID(char* ID,fstream& stream){
        fstream file1("dp.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        stream.close();
        stream.open("d.txt",ios::out|ios::in);
        PIndexDep* dep;
        dep=ReadPIndD(file1,r);
        int rrn=GetRecordByID(dep,ID,r);
        Department d=GetDepartment(rrn,stream);
        d.Get();
        file1.close();
    }

    int GetRecordByID(PIndexDep* dep,char* ID,int r){
        int low=0,high=r-1,mid;
        return Binarysearch(dep,ID,low,high,r);
    }

    int Binarysearch(PIndexDep* dep,char* ID,int b,int e,int r){
            int mid;
            while(b<=e){
                mid = (b + e) / 2;
                if(strcmp(ID,dep[mid].ID)==0){
                    return dep[mid].RRN;
                    break;
                    }
                if (ID < dep[mid].ID)
                    e = mid - 1;
                if (ID > dep[mid].ID)
                    b = mid + 1;
                }
            return -1;
    }

    void searchByDeptId(char* ID,fstream& stream){
        fstream file1("ds.txt",ios::out|ios::in);
        fstream file2("dl.txt",ios::out|ios::in);
        int r=numofRecords(stream);
        stream.close();
        stream.open("d.txt",ios::out|ios::in);
        SIndexDep* dep;
        dep=ReadSIndD(file1,r);
        int rrn=GetRecordsByName(dep,ID,r);
        LDep d;
        file2.seekg(rrn,ios::beg);
        do{
            file2.read((char*)&d,sizeof(d));
            searchByID(d.ID,stream);
            file2.seekg(d.next,ios::beg);
        }
        while(d.next!=-1);
        file1.close();
        file2.close();
    }

    int GetRecordsByName(SIndexDep* dep,char* Name,int r){
        int low=0,high=r-1,mid;
        return BinarysearchN(dep,Name,low,high,r);
    }

    int BinarysearchN(SIndexDep* dep,char* Name,int b,int e,int r){
            int mid;
            while(b<=e){
                mid = (b + e) / 2;
                if(strcmp(Name,dep[mid].Name)==0){
                    return dep[mid].RRN;
                    break;
                    }
                if (strcmp(Name , dep[mid].Name)<0)
                    e = mid - 1;
                if (strcmp(Name , dep[mid].Name)>0)
                    b = mid + 1;
                }
            return -1;
    }
};

int main()
{
    bool state=true;
    int x;
    while(state){
            cout<<"1 Insert New Emplyee"<<endl;
            cout<<"2 Insert New Department"<<endl;
            cout<<"3 Delete Emplyee(ID)"<<endl;
            cout<<"4 Delete Department(ID)"<<endl;
            cout<<"5 print Emplyee(ID)"<<endl;
            cout<<"6 print Emplyee(dept_ID)"<<endl;
            cout<<"7 print Department(ID)"<<endl;
            cout<<"8 print Department(Name)"<<endl;
            cout<<"9 Write Query"<<endl;
            cout<<"10 Exit"<<endl;
            cin>>x;
    switch(x){
        case 1:{
                fstream file("e.txt",ios::in|ios::out);
                Employee e;
                cin>>e;
                file.seekp(0,ios::end);
                if(file.tellp()==0){
                    int header=-1;
                    file.write((char*)& header,sizeof(int));
                }
                e.WriteEmployee(file);
                file.close();
                break;
        }
        case 2:{
            fstream file("d.txt",ios::in|ios::out);
            Department d;
            cin>>d;
            file.seekp(0,ios::beg);
            if(file.tellp()==0){
                int header=-1;
                file.write((char*)& header,sizeof(int));
            }
            d.WriteDeparment(file);
            file.close();
            break;
        }
        case 3:{
                fstream file("e.txt",ios::in|ios::out);
                Employee e;
                char c[13];
                cin>>c;
                e.DeleteEmployeeByID(c,file);
                file.close();
                break;
        }
        case 4:{
                fstream file("d.txt",ios::in|ios::out);
                Department d;
                char c[30];
                cin>>c;
                d.DeleteDepartmentByID(c,file);
                file.close();
                break;
        }
        case 5:{
                fstream file("e.txt",ios::in|ios::out);
                Employee e;
                char c[13];
                cin>>c;
                e.searchByID(c,file);
                file.close();
                break;
        }
        case 6:{
                fstream file("e.txt",ios::in|ios::out);
                Employee e;
                char c[13];
                cin>>c;
                e.searchByDeptId(c,file);
                file.close();
                break;
        }
        case 7:{
                fstream file("d.txt",ios::in|ios::out);
                Department d;
                char c[30];
                cin>>c;
                d.searchByID(c,file);
                file.close();
                break;
        }
        case 8:{
                fstream file("d.txt",ios::in|ios::out);
                Department d;
                char c[30];
                cin>>c;
                d.searchByDeptId(c,file);
                file.close();
                break;
        }
        case 9:{
            string x;
            cin.ignore();
            getline(cin,x);
            string select=x.substr(0,6);
            string all=x.substr(7,3);
            string EmpName=x.substr(7,13);
            string Emp=x.substr(16,8);
            string Emp1=x.substr(26,8);
            string Dep=x.substr(16,10);
            string DepID=x.substr(31,7);
            string DepID1=x.substr(29,7);
            string EmpID=x.substr(41,11);
            string IDDep;
            string IDDep1;
            string IDEmp;
            if(39<x.size()){
                IDDep=x.substr(39);
            }
            if(41<x.size()){
                IDDep1=x.substr(41);
            }
            if(53<x.size()){
                IDEmp=x.substr(53);
            }
            transform(select.begin(), select.end(), select.begin(), ::tolower);
            if(select.compare("select")==0){
                transform(all.begin(), all.end(), all.begin(), ::tolower);
                transform(EmpName.begin(), EmpName.end(), EmpName.begin(), ::tolower);
                if(all.compare("all")==0){
                    transform(Emp.begin(), Emp.end(), Emp.begin(), ::tolower);
                    transform(Dep.begin(), Dep.end(), Dep.begin(), ::tolower);
                    if(Emp.compare("employee")==0){
                        transform(DepID.begin(), DepID.end(), DepID.begin(), ::tolower);
                        if(DepID.compare("dept_id")==0){
                            fstream file("e.txt",ios::in|ios::out);
                            Employee e;
                            char id[30];
                            strcpy(id,IDDep.c_str());
                            e.searchByDeptId(id,file);
                            file.close();
                        }
                    }
                    else if(Dep.compare("department")==0){
                        transform(DepID1.begin(), DepID1.end(), DepID1.begin(), ::tolower);
                        if(DepID1.compare("dept_id")){
                            fstream file("d.txt",ios::in|ios::out);
                            Department d;
                            char id[30];
                            strcpy(id,IDDep1.c_str());
                            d.searchByID(id,file);
                            file.close();
                        }
                    }
                }
                else if(EmpName.compare("employee_name")==0){
                    transform(Emp1.begin(), Emp1.end(), Emp1.begin(), ::tolower);
                    if(Emp1.compare("employee")==0){
                                transform(EmpID.begin(), EmpID.end(), EmpID.begin(), ::tolower);
                                if(EmpID.compare("employee_id")==0){
                                    fstream file("e.txt",ios::in|ios::out);
                                    Employee e;
                                    char c[13];
                                    strcpy(c,IDEmp.c_str());
                                    e.searchByIDName(c,file);
                                    file.close();
                                }
                    }
                }
            }
            break;
        }
        case 10:{
            state=false;
            break;
        }
    }
    }
    return 0;
}
