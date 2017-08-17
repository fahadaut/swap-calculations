#ifndef _DATE_HEADER
#define _DATE_HEADER
#include <ctime>

using namespace std;
enum PERIOD {DAY=1,MONTH,YEAR};

class Cdate{
private:
       struct { 
           int year;
           int mon;
           int mday;
           } tm1;
      void remSpaces(string& str1); //removes whole of string          
      bool leapYear(int);           //if leapYear, then returns true
      void tolower(string& str1);   //converts given string to lower case   
      void dateFormat();            // suggests date format to user
      int checkDate() const ;       //checks date that date < 32 and month < 13
            
public:
      Cdate();                     //constructor
      Cdate(const string& adate);  //constructor
      Cdate(const int bdate);      //constructor
      void copy(Cdate& other){ tm1 = other.tm1;} //copy method
      Cdate(Cdate& other) {copy(other);}         //copy constructor
      int getdate()  { return tm1.mday;}         //returns date
      int getmonth() { return tm1.mon;}          //returns month
      int getyear()  { return tm1.year; }        //returns year
      void printDate() const;                    //prints date
      Cdate& operator + (int );                  //adds days to date
      Cdate& operator - (int );                  //subt days from date
      bool   operator == (const Cdate&) const;   //operator overloading
      bool   operator != (const Cdate&) const;   //operator overloading
      bool   operator <  (const Cdate&) const;   //operator overloading
      int    operator -  (const Cdate&) const;   //operator overloading 
      static void roll(Cdate&, PERIOD,int);  //it adds n periods to the given date, either days, months or years in a given date
      bool isEndOfMonth() const;     //checks of End of Month, last date
      int dayOfWeek() const;         //returns day of week sunday as 0
}; 
///////////////////////////////////////////////
///////////////// Implementation //////////////
      Cdate::Cdate() {
            struct tm tm2;
            time_t t1; time(&t1);
            tm2 = *localtime(&t1);
            tm1.year = tm2.tm_year+1900;
            tm1.mon = tm2.tm_mon+1;
            tm1.mday= tm2.tm_mday;
      }
      
      ///////////////////////////////////                   
      void Cdate::printDate() const {
           if (checkDate() < 0) 
              cout<<"Invalid Date..."<<endl;
           else    
           cout<<tm1.mday<<"/"<<tm1.mon<<"/"<<tm1.year<<" ";
      }                  
      ////////////////////////
      bool Cdate::leapYear(int year) {
             if ( (year % 400) == 0) return true;
             else if (((year % 4) == 0) && ((year % 100) !=0)) return true;
             else return false;
      }
      ///////////////////////////////
      bool Cdate::isEndOfMonth() const{
           int daysmon[13]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};     
           if (daysmon[tm1.mon] == tm1.mday) return true;
           else return false;
      }
      //////////////////////////////
      int Cdate::dayOfWeek() const {
          struct tm tma;
          tma.tm_year = tm1.year-1900; tma.tm_mon=tm1.mon-1; 
          tma.tm_mday=tm1.mday;
          tma.tm_hour=0; tma.tm_min=0; tma.tm_sec=0;
          mktime(&tma); 
          return tma.tm_wday;
      }
      //////////////////////////////
      void Cdate::roll( Cdate& date1, PERIOD typepd, int nperiods) {
           Cdate temp; 
           if (typepd == DAY) {
               date1 = date1 + nperiods;
           }
           else if (typepd == MONTH ) {
                date1.tm1.mon += nperiods;
                if (date1.tm1.mon > 12) {
                     date1.tm1.mon -= 12;
                     date1.tm1.year += 1;
                }                  
           }
           else if (typepd == YEAR) {
                date1.tm1.year += nperiods;
           }
           else cout<<"Period of roll is invalid."<<endl;     
      }     
      //////////////////////////////
      int Cdate::operator  -  (const Cdate& other) const {
             struct tm tma, tmb, tmdiff;
             time_t t1, t2, tdiff;
             
             tma.tm_year = tm1.year-1900; tma.tm_mon=tm1.mon-1; 
             tma.tm_mday=tm1.mday;
             tma.tm_hour=0; tma.tm_min=0; tma.tm_sec=0;
             tmb.tm_year = other.tm1.year-1900; tmb.tm_mon=other.tm1.mon-1; 
             tmb.tm_mday=other.tm1.mday;             
             tmb.tm_hour=0; tmb.tm_min=0; tmb.tm_sec=0; 
             t1 = mktime(&tma); t2 = mktime(&tmb);
             tdiff = t1 - t2;
             return tdiff/3600/24;
      }    
      //////////////////////////////
      Cdate& Cdate::operator + (int days ) {
            int daysmon[13]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};     
            int serDay=tm1.mday, serMon=tm1.mon, serYear=tm1.year;
            int j;
            if (leapYear(serYear)) daysmon[2]=29;
            for (j=1; j<=days; j++) {
                  serDay++;
                  if (serDay > daysmon[serMon]) {
                        serMon++; serDay=1;       
                        if (serMon > 12) {
                             serMon=1; serYear++; 
                             if (leapYear(serYear)) daysmon[2]=29;
                        }     
                  }           
            }    
            tm1.year = serYear; tm1.mon=serMon; tm1.mday=serDay;
            daysmon[2]=28;
            return *this;
      }    
      /////////////////////// 
      Cdate& Cdate::operator - (int days ) {
            int daysmon[13]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};     
            int serDay=tm1.mday, serMon=tm1.mon, serYear=tm1.year;
            int j;
            if (leapYear(serYear)) daysmon[2]=29;
            for (j=days; j > 0; j--) {
                  serDay--;
                  if (serDay < 1) {
                        serMon--; serDay=daysmon[serMon];
                        if (serMon < 1) {
                             serMon=12; serYear--; 
                             if (leapYear(serYear)) daysmon[2]=29;
                             serDay=daysmon[serMon];
                        }     
                  }           
            }    
            tm1.year = serYear; tm1.mon=serMon; tm1.mday=serDay;
            daysmon[2]=28;
            return *this;
      }    
      /////////////////////////////
      bool Cdate::operator==(const Cdate& other) const {
           if (tm1.year != other.tm1.year) return false;
           if (tm1.mon  != other.tm1.mon)  return false;
           if (tm1.mday != other.tm1.mday) return false;
           return true;
      }     
      //////////////////////////////
      bool Cdate::operator != (const Cdate& other) const {
           if (tm1.year != other.tm1.year) return true;
           if (tm1.mon  != other.tm1.mon)  return true;
           if (tm1.mday != other.tm1.mday) return true;
           return false;
      }  
      ///////////////////////////////////////////
      bool Cdate::operator < (const Cdate& other) const {
           if (tm1.year < other.tm1.year ) return true;
           else if (tm1.year == other.tm1.year) {
                if (tm1.mon  < other.tm1.mon)  return true;
                else if ( tm1.mon  == other.tm1.mon  ) {
                     if (tm1.mday < other.tm1.mday) return true;
                     else return false;
                }     
                else return false;
           }     
           else return false;
      }     
      //////////////////////////////////////////
      Cdate::Cdate(const string& adate) {   //date format 03/14/2010  or 03-dec-2011
            string temp1=adate, temp2;
            int posn1, posn2;
            //tm1.tm_hour=0; tm1.tm_min=0; tm1.tm_sec=0;
            //tm1.mday=0; tm1.mon=0; tm1.year=0;
            int j;
            string monthNames[]= { "dummy", "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug",
                             "sep", "oct", "nov", "dec"};
            //if (temp1.length() < 10) {dateFormat(); return;}
            remSpaces(temp1);  
            //get date
            posn1 = temp1.find_first_of('/');
            if (posn1 > 0) {
                  //get date    
                  tm1.mday= atoi((temp1.substr(0,posn1)).c_str());
                  //get month and year
                  posn1++;
                  posn2 = temp1.find_last_of('/');
                  tm1.mon= atoi((temp1.substr(posn1,posn2-posn1)).c_str());
                  tm1.mon;
                  posn2++;
                  tm1.year= atoi((temp1.substr(posn2,temp1.length())).c_str());
            }
            else  {
                 posn1=temp1.find_first_of('-'); 
                 //get date
                 tm1.mday= atoi((temp1.substr(0,posn1)).c_str());
                 //get month and year
                 posn1++;
                 posn2=temp1.find_last_of('-');
                 temp2 = temp1.substr(posn1,posn2-posn1); 
                 tolower(temp2); 
                 for(j=1; j<=12; j++) {
                     if (temp2.compare(monthNames[j]) == 0)  break;
                 }                   
                 tm1.mon=j; 
                 posn2++;
                 tm1.year= atoi((temp1.substr(posn2,temp1.length())).c_str());
            }
            if (checkDate() < 0) dateFormat();
      }   
      //////////////////////////////////////////
      Cdate::Cdate(const int bdate) {  //date format 20100326
           int temp1, temp2;
           temp1 = bdate/10000;
           tm1.year = temp1;
           temp1 = bdate - temp1 * 10000;
           temp2 = temp1/100;
           tm1.mon = temp2;
           temp1 = temp1 - temp2*100;
           tm1.mday = temp1;
           if (checkDate() < 0) dateFormat();
      }             
      ///////////////////////
      int Cdate::checkDate() const {
            if (tm1.mday < 1  ||  tm1.mday > 31)    return -1;
            if (tm1.mon  < 1  ||  tm1.mon  > 12)    return -1; 
            if (tm1.year < 1  ||  tm1.year > 5000)  return -1;
            return 0;
      }     
      ///////////////////// 
      void Cdate::remSpaces(string& str1) {
            string temp="";
            for(int j=0; j<str1.length(); j++) {
                  if (str1.at(j)==' ') continue;
                  temp.push_back(str1.at(j));
            }
            str1 = temp;
      }
      ///////////////////////
      void Cdate::tolower(string& str1) {
            string temp="";
            char ch;
            for (int j=0; j<str1.length(); j++) {
                ch = str1.at(j);
                if (ch > 64 && ch < 91) ch=ch+32;
                temp.push_back(ch); 
            }    
            str1=temp;
      }     
      ////////////////////////////////
      void Cdate::dateFormat() {
           tm1.mday=0; tm1.mon=0; tm1.year=0;  
           cout<<"Please use following date formats...\n \
           dd/mm/yyyy  or   dd-mon-yyyy, eg \n \
           03/14/2010  or   03-dec-2011\nAlso make sure date is within 1-31 and month within 1-12"<<endl;
           cout<<"If it is integer date, then it should be like 20100523"<<endl;
      }
#endif
