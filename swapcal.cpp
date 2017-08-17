#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "Cdate.h"
#include "stringLib.h"

using namespace std;
int dummy;
char startDate[11];  //start date of the project
//char swapStartDate[

/////////////////////////////////////
ifstream bfile;
ofstream logFile;

/////////////////////////////
class cswap {
      
 private:
  double years, months, days;
  int startYear, startMonth, startDay;       
  
  double tenor;  //tenor in yuears
  double Notionalprincipal;        //notional principal amount
  double fixedInt;  //fixed Interest expressed like 4.5%
  double liborRates[500];
  int totRates;
  char DayCountConv, PBasis; //1=annual, 2=semiannual or 3 etc, see config file
  string PBasiss;   //in strings like halfYearly, quarterly etc
  double DCfactor;  //Daycount Convention factor  
  double Pfactor;  //factor like 1, 2, 3 for payment basis
  char liborFile[64], logFilePath[64];
  Cdate date1, date2;
  string Currency;

  string getval();                          //for reading values from file
  void readlibor();                         //read liborrates file

  int getDays1(Cdate date1, Cdate date2);  //gets days on 365 days basis
  int getDays2(Cdate date1, Cdate date2);  //gets days on 360 days basis
  void WriteLog();   //writes log of activiites
public:   
  void readConfig();       //read configuration file          
   void docalc();         //do the detailed calculations
   void userint();        //user interface
};
  //////////////// Implementation of class swap ////////////////
  string cswap::getval()
  {
       string s1;
       size_t posn;
       getline (bfile,s1);
       posn = s1.find("//");
       return s1.substr(0, posn);
  }

  /////////////////////////
  void cswap::readlibor()
  {
         char  aline[80], bline[10], *ch1; 
         string str2;
         totRates=1;
         ifstream cfile;
         //char filename1 [80]="libor.txt";
         cfile.open(liborFile);
         if (! cfile.is_open()) {
             cout<<"Could not open "<<liborFile<<". Exiting..."<<endl;  
             system("PAUSE");  exit(1);
         }  

         while (! cfile.eof()) {
              getline (cfile,str2);
              //if (str2.length() == 0) break;
              //cout<<str2<<endl;
              strcpy(aline, str2.c_str());
              ch1 = strtok(aline,","); ch1=strtok(NULL,","); 
              liborRates[totRates] = atof(ch1);
              if (liborRates[totRates]==0) continue;
              cout<<totRates<<" ";
              cout<<liborRates[totRates]<<endl;
              totRates++;
        }      
        cfile.close(); cout<<--totRates<< " Items read from libor file."<<endl<<endl;
        
        //modify libor rates in the light of dayCount Convention             
  }
  ////////////////////////////////
  void cswap::readConfig()
  {
        string str2, str3;
        size_t posn1, posn2;
        char *ch1,  aDate[11], aline[64];
        ifstream lfile;
        
        //open configuration file
        cout<<"Reading Configuration File...."<<endl;
        char filename []="C:\swap-citiproject\solnfinal";
        bfile.open(filename);
        if (! bfile.is_open()) {
            cout<<"Could not open "<<filename<<". Exiting..."<<endl;  
            system("PAUSE");  exit(1);
        }  
        //File opened.... reading data...\n";
        //get tenor
        years=months=days=0.0;
        
        //get tenor years
        str2 = getval();  posn1 = str2.find_first_of('y');
        if (posn1 < 10) {      //if y not found
           str3 = str2.substr(0,posn1); years=(double)atof(str3.c_str()); 
        }   
        
        //get tenor months
        posn2 = str2.find_first_of('m'); posn1++;
        if (posn2 < 10) {
            str3 = str2.substr(posn1,posn2-posn1); months=(double)atof(str3.c_str()); 
        }    
        
        //get tendor days;
        posn1 = posn2+1; posn2 = str2.find_first_of('d');
        if (posn2 < 10) {
               str3 = str2.substr(posn1,posn2-posn1); days=(double)atof(str3.c_str()); 
        }       
        tenor = days/365.0/12.0 + months/12.0 + years;
                
        //get Fixed Interest Rate
        str2 = getval(); fixedInt = atof(str2.c_str());
        cout<<"Fixed Interest Rate = "<<fixedInt<<endl;
        
        //get start date
        str2 = getval();  strcpy(aDate, str2.c_str()); strcpy(startDate, aDate);

        //get Notional Prinicpal
        str2 = getval();  Notionalprincipal = atof(str2.c_str());
        cout<<"Notionalprincipal = "<<Notionalprincipal<<endl;
        
        //get payment basis (half yearly, quarterly etc)
        str2 = getval();  PBasis=str2[0];
        PBasiss = getPBasis1(PBasis); //this is Pbasis string like annual, halfyearly etc
        Pfactor = getPBasis2(PBasis); //1=annual 2=halfyear 3=quarter etc
        
        //get daycount convention
        str2 = getval(); DayCountConv=str2[0];

        //get currency
        Currency = getval();  
        
        //get libor rates file path
        str2 = getval(); strcpy(liborFile, str2.c_str());
        
        //get log file path
        str2 = getval(); strcpy(logFilePath, str2.c_str());
        bfile.close();
  }
  /////////////////////////////////
  int cswap::getDays1(Cdate date1, Cdate date2) {
      int ndays;
      ndays = date2 - date1;
      return ndays;
  }
  /////////////////////////////////
  int cswap::getDays2(Cdate date1, Cdate date2) {   //date2 is larger
     int ndays, d1, d2, m1, m2, y1, y2;
     d1 = date1.getdate(); m1=date1.getmonth(); y1= date1.getyear();
      ndays = 30-d1 + min(d2,30);
      ndays += (m2-m1-1) * 30;
      ndays += y2-y1 * 360;
      return ndays;
  }    
  /////////////////////////////////
  void cswap::docalc()
  {
    int t, ndays;    //t=ser.no of time periods   ndays between two dates
    double temp, df, PVfloating, PVfixed, sumfloating, sumfixed;
    double netval, periodpay;    
    double impval[500]={0.0}; //zoroth value will not be used, so zeroed out
    double nm;
    PERIOD typepd;  //in the roll function we add months, or years and days
    int npd; //how many months are days to be added in roll
    Cdate stDate(startDate);
    nm = Pfactor * tenor;  //number of periods
    cout<<"Number of Periods nm = "<<nm<<endl;
 
    cout<<"Reading libor rates....."<<endl;
        //used in roll function
    int iPBasis= (int)PBasis;
    switch (iPBasis) {
           case '1':  typepd=YEAR;   npd=1; break; //annual, add 1 year
           case '2':  typepd=MONTH;  npd=6; break; //6 monthly
           case '3':  typepd=MONTH;  npd=3; break; //quarterly  
           case '4':  typepd=MONTH;  npd=1; break; //monthly add 1 month
           case '5': typepd=MONTH;   npd=0; break; //give actual no of days  
    } 
    
    readlibor();  //read libor rates from file
    
    //modify libor rates with Day count convention
    int iDayCountConv = (int) DayCountConv;
    
    switch (iDayCountConv) {
           case '1':  DCfactor=1.0; break;          //actual/actual so no change
           case '2':  ndays=getDays1(date1, date2); DCfactor = (double) ndays/365.0; break;
           case '3':  ndays=getDays2(date1, date2); DCfactor = (double) ndays/360.0; break;          
    }       
    for(int j=1; j<=totRates; j++) {
           liborRates[totRates] = DCfactor * liborRates[totRates];
    }
   
    //calculate implied rates     
    for(t=1; t<=nm; t++) {         
        if (t==1) {
            impval[t]=liborRates[1];
            continue;
        }    
        temp=pow((1+liborRates[t]/200.0), (double) t);  //numerator  
        temp = temp/(1+ impval[1]/200.0);
        for (int k=2; k<t; k++) {
           temp=temp/(1+impval[k]/200); 
        }        
        temp = (temp - 1.0)*200.0;
        impval[t]=temp;
    }
    //do calculations
    cout<<"Doing Calculations.... Fixed Interest = "<<fixedInt<<"%"<<endl;
    
    df=1.0; sumfixed=0.0; sumfloating=0.0; netval=0.0;
    periodpay = Notionalprincipal * fixedInt/200.0; //periodic payment
    cout<<"Date       t rt/2 impVal   DF    PVfloat  PVfixed   Netval"<<endl;
    for(t=1; t<=nm; t++) {
       stDate.printDate();      
       cout<<setw(2)<<t<<" "<<setprecision(4)<<setw(5)<<liborRates[t]/2.0<<" "<<setprecision(4)<<setw(5)<<impval[t]/2.0;
       df = df/(1+impval[t]/200.0);
       
       cout<<"  "<<setprecision(4)<<setw(6)<<df;   
       PVfloating = Notionalprincipal * impval[t]/200.0 * df;
       sumfloating += PVfloating;
       cout<<" "<<setw(7)<<setprecision(6)<<PVfloating; 
       PVfixed = periodpay * df;
       sumfixed += PVfixed;
       cout<<"   "<<setw(5)<<setprecision(6)<<PVfixed; 
       cout<<"   "<<setw(5)<<setprecision(6)<<PVfloating-PVfixed<<endl; 
       netval += PVfloating-PVfixed;       
       Cdate::roll(stDate,typepd,npd); //roll the date
    }   
    cout<<"           summation =        "<<setw(10)<<setprecision(7)<<sumfloating<<"  "<<setprecision(7)<<sumfixed;
    cout<<"   "<<setprecision(7)<<netval<<endl;
    cout<<"\nFixed leg payer value        = "<<sumfloating-sumfixed<<endl;
    cout<<"Floating leg payer value     = "<<sumfixed-sumfloating<<endl;
    cout<<"(-ve value sign shows, loss for that party."<<endl;
    double swapRate = sumfloating/sumfixed * fixedInt;
    cout <<"\nSwap Rate (Fixed leg payer is ready to pay) = "<<swapRate<<"%"<<endl;      
         cout<<"\nType 0 and Hit <enter> key to continue....."<<endl;
         cin>>dummy; 
  }
  /////////////////////////////
  void cswap::WriteLog()
  {
       time_t t1;
       time(&t1);
       logFile.open (logFilePath, ios::app);
       
       logFile<<ctime(&t1);
       logFile<<"tenor="<<tenor << "years."<<endl;
       logFile<<"Start Date = "<<startDate<<endl;
       logFile<<"Payment Basis  = "<<PBasiss<<endl;
       logFile<<endl;       
       logFile.close();

       cout<<"Date written in log file.\n";
  }    
  ////////////////////////////////
  void cswap::userint()
  {
        char choice;
        while (1) {
              cout<<"|------------------------------------------------------------|"<<endl;
              cout<<"|                                                            |"<<endl;
              cout<<"|                     DATA                                   |"<<endl;
              cout<<"|                                                            |"<<endl;              
              cout<<"|     Tenor: years="<<years<<"  months="<<months<<"  days="<<days<<"                       |"<<endl;
              cout<<"|     PaymentBasis "<<setw(10)<<PBasiss<<"                                |"<<endl;
              cout<<"|     Notional Amount="<<setw(10)<<setprecision(10)<<Notionalprincipal<<"                             |"<<endl;
              cout<<"|     LIBOR Data File="<<liborFile<<"|"<<endl;      
              cout<<"|                                                            |"<<endl;
              cout<<"|                    Main Menu                               |"<<endl;
              cout<<"|                                                            |"<<endl;              
              cout<<"|     1. Write Log File                                      |"<<endl;
              cout<<"|     2. Read Configuration file                             |"<<endl;
              cout<<"|     3. Do Swap Calculations with above data                |"<<endl;
              cout<<"|     4. Exit from Program.                                  |"<<endl;
              cout<<"|                                                            |"<<endl;              
              cout<<"|     Enter your choice (1 to 4)..... ";
              cin>>choice;
              
              switch (choice) {
                     case '1':  cout<<"\nWrite Log\n"; WriteLog(); break;
                     case '2':  cout<<"Read Config\n"; readConfig(); break;
                     case '3':  cout<<"Calculations:\n"; docalc(); break;
                     case '4':  cout<<"Exiting...\n"; break;
                     default: cout<<"Plz type valeus 1 to 4...";
              }       
              if (choice=='4') break;
        }       
  }

//////////////////////////////
int main()
{
    cswap swap1;
    swap1.readConfig();
    swap1.userint();

    return EXIT_SUCCESS;
}
