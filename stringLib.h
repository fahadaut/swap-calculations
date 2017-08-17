#ifndef _STRINGLIB_HEADER
#define _STRINGLIB_HEADER

////////////////////// string library /////////////////
//////Note this has not been written as a class. Instead it is a library of functions.

//This function returns Annual, halfyearly etc, when give a character 1 to 5
string getPBasis1(char Pmb)
{
       string temp;
       switch(Pmb){
           case '1': return "Annual"; break;
           case '2': return "HalfYearly"; break;
           case '3': return "Quarterly"; break;
           case '4': return "Monthly"; break;
           case '5': return "Daily"; break;
           default: cout<<"Invalid value for Payment Basis. should be 1 to 5"<<endl;        
       }
}       
//This function returns a factor for fining number of paying periods, when give a character 1 to 5
double getPBasis2(char Pmb)
{
       switch(Pmb){
           case '1':  return 1.0; break;   //yearly
           case '2':  return 2.0; break;    //semiannually
           case '3':  return 4.0; break;    //quarterly
           case '4':  return 12.0; break;      //monthly  
           case '5':  return 365.0; break; 
           default: cout<<"Invalid value for Payment Basis. should be 1 to 5"<<endl;        
       }
}
//change date s1=02/11/2010   to s2=20101102       
void chgFormat(char *s1, char *s2)
{
       char * ch1, ch[3], ch2[3], ch3[3], adate[9];
       strcpy(adate, s1);
       //cout<<"in chgformat s1="<<s1<<"  s2="<<s2<<endl;       
       ch1 = strtok(adate, "/"); strcpy(ch2, ch1);  //day
       if (strlen(ch2) < 2) { 
            strcpy(ch,"0"); strcat(ch,ch1); strcpy(ch2,ch);
       }     

       ch1 = strtok(NULL, "/"); strcpy(ch3, ch1);   //month
       if (strlen(ch3) < 2) { 
            strcpy(ch,"0"); strcat(ch,ch3); strcpy(ch3,ch);
       }     

       ch1 = strtok(NULL, "/"); //year
       ch1[4]='\0';
       strcpy(s2,ch1); strcat(s2, ch3); strcat(s2, ch2);
       s2[10]='\0';
}

#endif
