#include <iostream>
#include <fstream>
#include <cstring>
#include <array>
#include <algorithm>

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

bool compare( PIndexEmp a, PIndexEmp b){
	if(a.ID < b.ID)
		return 1;
	else
		return 0;
}

void WritePrimaryIndexEmp(fstream& stream,int r,PIndexEmp e){
    stream.seekg(0,ios::end);
    PIndexEmp* emp=new PIndexEmp[r];
    if(stream.tellg()==0){
        stream.write((char*)&e,sizeof(e));
        stream.put('$');
    }
    else{
        stream.seekg(0,ios::beg);
        PIndexEmp em;
        int i=0;
        while(stream.getline((char*)&em,17,'$')){
            emp[i]=em;
            i++;
        }
        emp[r-1]=e;
        sort(emp,emp+r,compare);
        stream.seekp(0,ios::beg);
        for(int i=0;i<r;i++){
            stream.write((char*)&emp[i],sizeof(emp[i]));
            stream.put('$');
            stream.seekp(0,ios::end);
        }
    }
}

void WritePrimaryIndexDep(fstream& stream,int r,PIndexDep d){
    stream.seekg(0,ios::end);
    PIndexDep* dep=new PIndexDep[r];
    if(stream.tellg()==0){
        stream.write((char*)&d,sizeof(d));
        stream.put('$');
    }
    else{
        stream.seekg(0,ios::beg);
        PIndexDep de;
        int i=0;
        while(stream.getline((char*)&de,34,'$')){
            dep[i]=de;
            i++;
        }
        dep[r-1]=d;
        sort(dep,dep+r);
        for(int i=0;i<r;i++){
            stream.write((char*)&dep[i],sizeof(dep[i]));
            stream.put('$');
        }
    }
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

    int WriteEmployee(fstream& stream,fstream& p){
        RRN=-1;
        int firstDeleted=-1,nextDeleted=-1;
        PIndexEmp e;
        stream.seekg(0,ios::beg);
        stream.read((char*)&firstDeleted,sizeof(int));
        RRN=firstDeleted;
        if(RRN==-1){
            stream.seekp(0,ios::end);
            RRN=stream.tellp();
        }
        else{
            stream.seekg(sizeof(int)+RRN,ios::beg);
            char c;
            stream.get(c);
            if(c!='*'){
                return -1;
            }
            stream.read((char*)&nextDeleted,sizeof(int));
            stream.seekp(0,ios::beg);
            stream.write((char*)&nextDeleted,sizeof(int));
            stream.seekp(sizeof(int)+RRN,ios::beg);
        }
        e.RRN=RRN;
        strcpy(e.ID,Employee_ID);
        Write(stream);
        int r=numofRecords(stream);
        WritePrimaryIndexEmp(p,r,e);
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

    int numofRecords(fstream& stream){
        int count=0;
        char c[156];
        while(stream.getline(c,156,'$')){
            count++;
        }
        return count;
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
        RRN=-1;
        int firstDeleted=-1,nextDeleted=-1;
        stream.seekg(0,ios::beg);
        stream.read((char*)&firstDeleted,sizeof(int));
        RRN=firstDeleted;
        if(RRN==-1){
            stream.seekp(0,ios::end);
            RRN=stream.tellp();
        }
        else{
            stream.seekg(sizeof(int)+RRN,ios::beg);
            char c;
            stream.get(c);
            if(c!='*'){
                return -1;
            }
            stream.read((char*)&nextDeleted,sizeof(int));
            stream.seekp(0,ios::beg);
            stream.write((char*)&nextDeleted,sizeof(int));
            stream.seekp(sizeof(int)+RRN,ios::beg);
        }
        Write(stream);
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

    int numofRecords(fstream& stream){
        int count=0;
        char c[148];
        while(stream.getline(c,148,'$')){
            count++;
        }
        return count;
    }
};

int main()
{
    Employee e;
    cin>>e;
    fstream file("e.txt",ios::out|ios::in);
    fstream file1("ep.txt",ios::out|ios::in);
//    file.seekp(0,ios::end);
//    int header=-1;
//    file.write((char*) &header,sizeof(int));
    int r=e.WriteEmployee(file,file1);
    cout<<r;

//    Department d;
//    cin>>d;
//    fstream file("d.txt",ios::out|ios::in);
//    file.seekp(0,ios::end);
//    int header=-1;
//    file.write((char*) &header,sizeof(int));
//    int r=d.WriteDeparment(file);
//    cout<<r;

//    Employee e;
//    fstream file("e.txt",ios::out|ios::in);
//    Employee r=e.GetEmployee(4,file);
//    r.Get();

//    Department d;
//    fstream file("d.txt",ios::out|ios::in);
//    Department r=d.GetDepartment(32,file);
//    r.Get();

//    Employee e;
//    fstream file("e.txt",ios::out|ios::in);
//    if(e.DeleteEmployee(4,file));

//    Department d;
//    fstream file("d.txt",ios::out|ios::in);
//    if(d.DeleteDepartment(4,file));

//    Employee e;
//    fstream file("e.txt",ios::out|ios::in);
//    cout<<e.numofRecords(file);

//    Department d;
//    fstream file("d.txt",ios::out|ios::in);
//    cout<<d.numofRecords(file);

    return 0;
}
