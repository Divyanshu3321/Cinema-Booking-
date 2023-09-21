#include <iostream>
#include <string.h>
#include <fstream>
#include <limits>
#include <cstdlib> // for system("cls")
#include <iomanip> // Include the header for setw
#include <vector>
using namespace std;

class SeeMovieDetails{
    int movieid;
    char moviedate[20];
    char movietime[20];
    char moviename[200] , hallname[200]; 

    public:
    SeeMovieDetails() {
        movieid=0;
        strcpy(this->moviename, "");  // full string udayaga
        strcpy(this->hallname, "");
        strcpy(this->moviedate, "");
        strcpy(this->movietime, "");
    }
    SeeMovieDetails(int movieid, char moviename[], char hallname[], char moviedate[], char movietime[]) {
        this->movieid=movieid;
        strcpy(this->moviename,moviename);
        strcpy(this->hallname,hallname);
        strcpy(this->moviedate,moviedate);
        strcpy(this->movietime,movietime);
    }
    void displaymovie(){
        cout<<"\n---------------------------------------------------------------------";
        cout<<"\nMovie ID: "<<movieid;
        cout<<"\nMovie Name: "<<moviename;
        cout<<"\nHall of Movie: "<<hallname;
        cout<<"\nMovie Date: "<<moviedate;
        cout<<"\nMovie Time: "<<movietime;
        cout<<"\n---------------------------------------------------------------------";
    }
    bool isValidTimeFormat(const char *timeStr) {
        int hour, minute, second;
        if (sscanf(timeStr, "%d:%d:%d", &hour, &minute, &second) != 3) {
            return false;
        }
        if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
            return true;
        }
        return false;
    }
    bool search(int movieid){
        SeeMovieDetails searchmovie;
        ifstream file("Cinema_Booking_System.dat",ios::app);   //read from binary file
        
        while(file.read((char*)&searchmovie,sizeof(searchmovie)))
        {
            if (movieid==searchmovie.movieid){ 
                return true;
            }
        }
            return false;
        file.close();

    }

    void search(){
        SeeMovieDetails searchmovie; // calling default constructor
        char enteredmoviename[20];
        int result=1;
        cout<<"Enter Movie Name: ";
        cin>>enteredmoviename;
        ifstream file("Cinema_Booking_System.dat",ios::app);   //read from binary file
        while(file.read((char*)&searchmovie,sizeof(searchmovie)))
        {
            if (strcmp(searchmovie.moviename,enteredmoviename)== 0){  //comparing two strings
                result=0;
                // cout<<"Found";
                searchmovie.displaymovie();
            }
        }
        if(result==1){
            cout<<"Movie Not Found ";
        }
        file.close();

    }
    void insertingmovie()
    {
        SeeMovieDetails seedetails;
        int movieid, result;
        char option[20];
        for (;;)
        {
            ofstream file("Cinema_Booking_System.dat", ios::app); // open and write in Binary file
            cout << "Movie ID: ";
            while (!(cin >> movieid)) {
            cout << "Invalid input. Please enter an integer for Movie ID: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Name Of Movie: ";
            cin >> moviename;
            cout << "Movie in Which Hall: ";
            cin >> hallname;
            int day, month, year;
            cout << "Date of Movie (DD/MM/YYYY): ";
            char dateStr[11];
            cin >> dateStr;

            if (sscanf(dateStr, "%d/%d/%d", &day, &month, &year) != 3) {
                cout << "Invalid date format. Please use DD/MM/YYYY format." << endl;
                return;
            }
            char movietimeStr[9];  // To store the time as a string
            cout << "Time of Movie (HH:MM:SS): ";
            cin >> movietimeStr;

            if (!isValidTimeFormat(movietimeStr)) {
                cout << "Invalid time format. Please use HH:MM:SS format." << endl;
                return;
            }
            bool result =seedetails.search(movieid);
            if(result==1){
                cout<<"Duplicate ID (Movie Id Already present)";
                break;}
            else{ 
                SeeMovieDetails objseemoviedetails(movieid, moviename, hallname, dateStr,movietimeStr );   // parametrized constructor passing
                file.write((char *)&objseemoviedetails, sizeof(objseemoviedetails)); // inserting object address and object size in file
                cout << "Do You Want to Continue: Yes/No ";
                cin >> option;
                if (strcmp(option, "No") == 0)
                { // Compare strings
                    cout << "Exit Status Achieved\n";
                    break;
                }
                else if (strcmp(option, "Yes") == 0)
                {
                    cout << "-----------------------------------------\n";
                }
                else
                {
                    cout << "IN Valid Option";
                    break;
                }
            }
            file.close();

        }
    }
    void deletemovie()
    {
        int movieid;
        SeeMovieDetails seedetails;
        cout<<"Enter Movie ID: ";
        cin>>movieid;
        bool result =seedetails.search(movieid);
        // cout<<result;
        if(result==0){
            cout<<"Movie Not Found";
            return;
        }
        else{
        ifstream file("Cinema_Booking_System.dat", ios::app); // read from app file
        ofstream fileo("Cinema_garbage_File.dat", ios::app);  // opening temprary made file // and writing in it( isme data ko change karaga then delete old file rename this file name to old dile name )
        while (file.read((char *)&seedetails, sizeof(seedetails)))
        {
            if (seedetails.movieid != movieid)
            {
            fileo.write((char*)&seedetails, sizeof(seedetails));  // this is parametrized only it is filled when loop is runned
            }
                
        }
        fileo.close();
        file.close();
        remove("Cinema_Booking_System.dat");// purana data remove krange
        rename("Cinema_garbage_File.dat","Cinema_Booking_System.dat");  //temporray file ko rename kiya purani file ma thats trick
        }
    }
    void updatemovie()
    {
        SeeMovieDetails seedetails;
        int enteredmovieid;
        cout<<"Enter Movie ID: ";
        cin>>enteredmovieid;
        bool result =seedetails.search(enteredmovieid);
        // cout<<result;
        if(result==0){
            cout<<"Not found";
            return;
        }
        ifstream file("Cinema_Booking_System.dat",ios::app);   //read from app file
        ofstream fileo("Cinema_Garbage_File.dat", ios::app);  // making temporary file jisme changes karaga hum and then isko original ki jga replace krr denge hum 
        while(file.read((char*)&seedetails,sizeof(seedetails))){
            if(enteredmovieid == seedetails.movieid){
                char newmoviename[20];
                char newhallname[20];
                cout<<"\n--------------------------------------------------------------------\n";
                cout << "Name Of Movie: ";
                cin >> newmoviename;
                cout << "Movie in Which Hall: ";
                cin >> newhallname;
                int day, month, year;
                cout << "Date of Movie (DD/MM/YYYY): ";
                char newdateStr[11];
                cin >> newdateStr;

                if (sscanf(newdateStr, "%d/%d/%d", &day, &month, &year) != 3) {
                    cout << "Invalid date format. Please use DD/MM/YYYY format." << endl;
                    return;
                }
                char newmovietimeStr[9];  // To store the time as a string
                cout << "Time of Movie (HH:MM:SS): ";
                cin >> newmovietimeStr;

                if (!isValidTimeFormat(newmovietimeStr)) {
                    cout << "Invalid time format. Please use HH:MM:SS format." << endl;
                    return;
                }
                cout<<"\n--------------------------------------------------------------------\n";
                SeeMovieDetails ovbseemoviedet(enteredmovieid, newmoviename, newhallname, newdateStr,newmovietimeStr );
                fileo.write((char*)&ovbseemoviedet, sizeof(ovbseemoviedet)); 
                }
            else{
                fileo.write((char*)&seedetails, sizeof(seedetails));
                }
                }
                fileo.close();
                file.close();
                remove("Cinema_Booking_System.dat");// purana data remove krange
                rename("Cinema_Garbage_File.dat","Cinema_Booking_System.dat");
    }
    void showmovies()
    {
        SeeMovieDetails seedetails;
        ifstream file("Cinema_Booking_System.dat", ios::app); // read from binary file
        while (file.read((char *)&seedetails, sizeof(seedetails)))
        {
            seedetails.displaymovie();
        }
    }
    friend class CheckSeatsAvailability;   // friend banana sa we can access other class properties
    friend class BookTickets;
};


// cout<<"------------------------------------------------------------------------------";



class CheckSeatsAvailability{
    public:
    int diamondtotalrows, diamondtotalcolumns, amountdiamond;
    int silvertotalrows, silvertotalcolumns, amountsilver;
    int goldtotalrows, goldtotalcolumns, amountgold;
    int diamondgap, silvergap, goldgap;
    // int seats;
    char  hallname[200]; 

    public:
    CheckSeatsAvailability() {
        diamondtotalrows=0;
        diamondtotalcolumns=0;
        diamondgap=0;
        amountdiamond=0;
        strcpy(this->hallname, "");
        goldtotalcolumns=0;
        goldtotalrows=0;
        goldgap=0;
        amountgold=0;
        strcpy(this->hallname, "");
        silvertotalrows=0;
        silvertotalcolumns=0;
        silvergap=0;
        amountsilver=0;
        strcpy(this->hallname, "");
  
    }
    CheckSeatsAvailability(int diamondtotalrows, int diamondtotalcolumns, int diamondgap, int amountdiamond, int goldtotalrows, int goldtotalcolumns, int goldgap,int amountgold,int silvertotalrows, int silvertotalcolumns, int silvergap,  int amountsilver, char hallname[]) {
        this->diamondtotalrows=diamondtotalrows;
        this->diamondtotalcolumns=diamondtotalcolumns;
        this->diamondgap=diamondgap;
        this->amountdiamond=amountdiamond;
        // strcpy(this->hallname,hallname);
        this->goldtotalrows=goldtotalrows;
        this->goldtotalcolumns=goldtotalcolumns;
        this->goldgap=goldgap;
        this->amountgold=amountgold;
        // strcpy(this->hallname,hallname);
        this->silvertotalrows=silvertotalrows;
        this->silvertotalcolumns=silvertotalcolumns;
        this->silvergap=silvergap;
        this->amountsilver=amountsilver;
        strcpy(this->hallname,hallname);
    }
    void displayseats(){
        cout<<"\nDiamond rows: "<<diamondtotalrows;
        cout<<"\nDiamond columns: "<<diamondtotalcolumns;
        cout<<"\nDiamond gap: "<<diamondgap;
        cout<<"\nAmount Diamond: "<<amountdiamond;
        cout<<"\ngold rows: "<<goldtotalrows;
        cout<<"\ngold columns: "<<goldtotalcolumns;
        cout<<"\ngold gap: "<<goldgap;
        cout<<"\nAmount Gold: "<<amountgold;
        cout<<"\nsilver rows: "<<silvertotalrows;
        cout<<"\nsilver columns: "<<silvertotalcolumns;
        cout<<"\nsilver gap: "<<silvergap;
        cout<<"\nAmount Silver: "<<amountsilver;
        cout<<"\nHall Name: "<<hallname;



    }
    

    bool search(char enteredhallname[]){
        SeeMovieDetails searchhall;
        strcpy(enteredhallname,enteredhallname);
        ifstream file("Cinema_Booking_System.dat",ios::app);   //read from binary file
        
        while(file.read((char*)&searchhall,sizeof(searchhall)))
        {
            if (strcmp(enteredhallname,searchhall.hallname)==0){
            cout<<enteredhallname<<"\n";
            cout<<searchhall.hallname; 
                return true;
            }
        }
            return false;
        file.close();

    }
    void insertingseats()
    {
        CheckSeatsAvailability searchhall;
        int result;
        char enteredhallname[20];
        {
            ofstream file("Check_Seats_Availability.dat", ios::app); // open and write in Binary file
            cout << "Hall Name: ";
            cin>>enteredhallname;
            strcpy(hallname,enteredhallname);
            bool result =searchhall.search(enteredhallname);
            // cout<<result;
            if(result==1){
                char option[6];
                cout<<"Diamond Class: ";
                    cout<<"\nTotal Number of Rows: ";
                    cin>>diamondtotalrows;
                    cout<<"\nTotal Number of columns: ";
                    cin>>diamondtotalcolumns;
                    cout<<"\nAfter How many columns u want gap: ";
                    cin>>diamondgap;
                    cout<<"\nEnter Amount of Diamond Seats: ";
                    cin>>amountdiamond;
                    if(diamondtotalcolumns>diamondgap){
                        cout<<"\nInserted Successfully";
                    }
                    else{
                        cout<<"\nIn Valid Gap is more than Columns: ";
                    }

                cout<<"\nGold Class  ";
                    cout<<"\nTotal Number of Rows: ";
                    cin>>goldtotalrows;
                    cout<<"\nTotal Number of columns: ";
                    cin>>goldtotalcolumns;
                    cout<<"\nAfter How many columns u want gap: ";
                    cin>>goldgap;
                    cout<<"\nEnter Amount of Gold Seats: ";
                    cin>>amountgold;
                    
                    if(goldtotalcolumns>goldgap){
                        cout<<"\nInserted Successfully";
                    }
                    else{
                        cout<<"\nIn Valid Gap is more than Columns: ";
                    }

                cout<<"\nSilver Class ";
                    cout<<"\nTotal Number of Rows: ";
                    cin>>silvertotalrows;
                    cout<<"\nTotal Number of columns: ";
                    cin>>silvertotalcolumns;
                    cout<<"\nAfter How many columns u want gap: ";
                    cin>>silvergap;
                    cout<<"\nEnter Amount of Silver Seats: ";
                    cin>>amountsilver;
                    
                    if(silvertotalcolumns>silvergap){
                        cout<<"\nInserted Successfully";
                    }
                    else{
                        cout<<"\nIn Valid Gap is more than Columns: ";
                    }
                CheckSeatsAvailability searchhall( diamondtotalrows,  diamondtotalcolumns,  diamondgap,amountdiamond, goldtotalrows,  goldtotalcolumns,  goldgap, amountgold, silvertotalrows,  silvertotalcolumns,  silvergap, amountsilver,  hallname);
                file.write((char *)&searchhall, sizeof(searchhall));
            }
        else{ 
            cout<<"\nIn Valid Hall. Hall Doesn't Exist\n";
        }
                file.close();

            }
        }
    void showseats()  //// showing seats in binary file but afterwards we changed this code into text files which is written at the end of this class
    {
    CheckSeatsAvailability seedetails;
    int count ;
    ifstream file("Check_Seats_Availability.dat", ios::app); // read from binary file
    while (file.read((char *)&seedetails, sizeof(seedetails)))
        {
            // seedetails.displayseats();
        count=1;
    cout<<"\nHall Name: "<<seedetails.hallname<<"\n";
    cout<<"\n***********************************Diamond Class************************************\n";
    for (int i = 1; i <= seedetails.diamondtotalrows; i++) {
        for (int j = 1; j <= seedetails.diamondgap; j++) {
            cout << count++ << setw(5);
        }
        cout << setw(10);
        for (int j = 1; j <= seedetails.diamondtotalcolumns - seedetails.diamondgap; j++) {
            cout << count++ << setw(5);
        }
        cout << "\n";
    }
    cout<<"\n***********************************Gold Class************************************\n";
    for (int i = 1; i <= seedetails.goldtotalrows; i++) {
        for (int j = 1; j <= seedetails.goldgap; j++) {
            cout << count++ << setw(5);
        }
        cout << setw(10);
        for (int j = 1; j <= seedetails.goldtotalcolumns - seedetails.goldgap; j++) {
            cout << count++ << setw(5);
        }
        cout << "\n";
    }
    cout<<"\n***********************************Silver Class************************************\n";

    for (int i = 1; i <= seedetails.silvertotalrows; i++) {
        for (int j = 1; j <= seedetails.silvergap; j++) {
            cout << count++ << setw(5);
        }
        cout << setw(10);
        for (int j = 1; j <= seedetails.silvertotalcolumns - seedetails.silvergap; j++) {
            cout << count++ << setw(5);
        }
        cout << "\n";
    }
    
    }
    }
    friend class BookTickets;

};


// cout<<"------------------------------------------------------------------------------------------"


class CheckSeatsAvailability_in_text_file{
    public: 
    
    void insertseats()
    {
    CheckSeatsAvailability seedetails;
    int count ,i,j;
    ifstream fileo("Check_Seats_Availability.dat", ios::app); // read from binary file
   
    while (fileo.read((char *)&seedetails, sizeof(seedetails)))
    {
        char hall[20];
        strcpy(hall,seedetails.hallname);
        strcat(hall,".txt");
        ofstream write_in_file(hall);   // Use trunc to clear existing content 

        int diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns];  // accessing particular values inside the object
        int goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns];
        int silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns];
        // cout<<seedetails.goldtotalrows<<"\n";  //// printing to see wheather values are printed or not 
        // cout<<seedetails.goldtotalcolumns;
        
        count=1;
        // write_in_file<<"\nHall Name: "<<seedetails.hallname<<"\n";
        // write_in_file<<"\n***********************************Diamond Class************************************\n";
        
    for (int i = 1; i <= seedetails.diamondtotalrows; i++) {
        for (int j = 1; j <= seedetails.diamondgap; j++) {
            diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns]=count++;
            // cout<<diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns]<<" "; ////printing on console
            write_in_file << diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns] << " ";
        }
        // cout << setw(10);
        // write_in_file << setw(10);
        for (int j = 1; j <= seedetails.diamondtotalcolumns - seedetails.diamondgap; j++) {
            diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns]=count++;
            // cout<<diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns]<<" "; ////printing on console
            write_in_file << diamondmatrix[seedetails.diamondtotalrows][seedetails.diamondtotalcolumns] << " ";
        }
        // cout << "\n";
        write_in_file << "\n";
        }

    //    write_in_file<<"\n***********************************Gold Class************************************\n";
        for (int i = 1; i <= seedetails.goldtotalrows; i++) {
        for (int j = 1; j <= seedetails.goldgap; j++) {
            goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns]=count++;
            // cout<<goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns]<<" "; ////printing on console // cout means printing in console
            write_in_file << goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns] << " ";
        }
        // cout << setw(10);
        // write_in_file << setw(10);
        for (int j = 1; j <= seedetails.goldtotalcolumns - seedetails.goldgap; j++) {
            goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns]=count++;
            // cout<<goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns]<<" "; ////printing on console
            write_in_file << goldmatrix[seedetails.goldtotalrows][seedetails.goldtotalcolumns] << " ";
        }
        // cout << "\n";
        write_in_file << "\n";
        }

    //    write_in_file<<"\n***********************************Silver Class************************************\n";
        for (int i = 1; i <= seedetails.silvertotalrows; i++) {
        for (int j = 1; j <= seedetails.silvergap; j++) {
            silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns]=count++;
            // cout<<silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns]<<" "; ////printing on console
            write_in_file << silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns] << " ";
        }
        // cout << setw(10);   
        // write_in_file << setw(10);
        for (int j = 1; j <= seedetails.silvertotalcolumns - seedetails.silvergap; j++) {
            silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns]=count++;
            // cout<<silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns]<<" "; ////printing on console
            write_in_file << silvermatrix[seedetails.silvertotalrows][seedetails.silvertotalcolumns] << " ";
        }
        // cout << "\n";
        write_in_file << "\n";
        write_in_file.close();

        }
        }
        fileo.close();
        }
       
    void removeDupWord(string str,int gap)
    {
        string word = "";
        for (size_t i = 0; i < str.length(); i++)
        {
            char x = str[i]; // Explicitly declare 'x' as 'char'
            if (x == ' ')
            {
                cout << setw(5) << word << setw(5);
                word = "";
            }
            else {
                word = word + x;
            }
        }
        cout << word << endl;
    }

    void readingfromtextfile(){
        char hallname[20];
        cout<<"Movie Hallname: ";
        cin>>hallname;
        strcat(hallname,".txt");
        string line;
        ifstream fin(hallname);
        
    CheckSeatsAvailability_in_text_file ob1;
    CheckSeatsAvailability ob;
    // char hallname[]="ABCD";
    ob=ob1.getseatdetails(hallname);
    // ob.displayseats();
        int x=1;
        while(getline(fin,line))
        {
            if(x==1)
            {
                cout<<"\n ****************************Diamond Seats****************************** \n";
            }
            else if(x==ob.diamondtotalrows+1)
            {
                cout<<"\n ****************************Gold Seats****************************** \n";
            }
            
            else if(x==ob.diamondtotalrows+1+ob.goldtotalrows)
            {
               cout<<"\n ****************************Silver Seats****************************** \n";
            }
            removeDupWord(line,ob.diamondgap);
            x++;
            
        }
        fin.close();
        getseatdetails(hallname);
        }

        CheckSeatsAvailability getseatdetails(char hallname[]){
        ifstream file("Check_Seats_Availability.dat",ios::app);   //read from binary file
        CheckSeatsAvailability seedetails;
        int result=0;
        while(file.read((char*)&seedetails,sizeof(seedetails)))
        {   
            // cout<<hallname;
            if(strcasecmp(hallname,seedetails.hallname)==0){
                result=1;
                // cout<<"Found";
                break;
            }
               
        }
        if(result==0){
            // cout<<"Not Found";
        }

        file.close();
        // cout<<CheckSeatsAvailability.seedetails;
        return seedetails;
    }

};


// cout<<"------------------------------------------------------------------------------------------





class BookTickets{
    public:
    int numoftickets;
    string numofseats[10];
    char customername[20], moviename[20], hallname[20];
    int customerphone;

    public:
    BookTickets() {
        strcpy(this->moviename, "");  // full string udayaga
        strcpy(this->hallname, "");  // full string udayaga
        numoftickets=0;
        for(int i=0; i< numoftickets; i++){
            numofseats[i]="0";
        }
        strcpy(this->customername, "");  // full string udayaga
        customerphone=0;

    }
    BookTickets(char hallname[],char moviename[], int numoftickets, string numofseats[], char customername[], int customerphone) {
        strcpy(this->hallname,hallname);
        strcpy(this->moviename,moviename);
        this->numoftickets=numoftickets;
        for(int i=0; i< numoftickets; i++){
            this->numofseats[i]=numofseats[i];
        }
        strcpy(this->customername,customername);
        this->customerphone=customerphone;
    }
    void display(){
        cout<<"\n---------------------------------------------------------------------";
        cout<<"\nHall Name: "<<hallname;
        cout<<"\nMovie Name: "<<moviename;
        cout<<"\nNumber of Tickets: "<<numoftickets;
        cout<<"\nSeats Reserved: ";
        for(int i=0;i<numoftickets;i++){
            this->numofseats[i]=numofseats[i];
            cout<<numofseats[i];
        }
        cout<<"\nCustomer Name: "<<customername;
        cout<<"\nCustomer Phone: "<<customerphone;
        cout<<"\n---------------------------------------------------------------------";
    }

    void bookingseats(){
        CheckSeatsAvailability_in_text_file check;
        BookTickets seedetails;
        check.readingfromtextfile();
        cout<<"\n";
        cout<<"\n";
        cout<<"\n";
        cout<<"\n";
        ofstream file("Cinema_Seats_Booking.dat", ios::app); // open and write in Binary file
        cout<<"Movie Hallname: ";
        cin>>hallname;
        cout<<"Movie Name: ";
        cin>>moviename;
        cout<<"Number of Tickets(Maximum Tickets 10): ";
        cin>>numoftickets;
        if(numoftickets>10){
            cout<<"Ticket Count Exceeded";
            return;
        }
        else{
            cout<<"Enter Seat Numbers U Want to reseve: ";
            for(int i=0;i<numoftickets;i++){
                    cin>>numofseats[i];
                }
                for(int i=0;i<numoftickets;i++){
                    if(numofseats[i]==numofseats[i+1]){
                        cout<<"Seats are Repeated";
                        return;
                    }
                }
                
                    // string n[i] = numofseats[i];
                    string word,line,temp="";
                    
                    strcat(hallname,".txt");
                    ifstream read_file(hallname);   
                    ofstream write_file("Garbage.txt");
                    while(getline(read_file,line))
                    {
                        temp="";
                        for(int i = 0; i < line.length(); i++)
                        {
                            if(line[i] == ' ')
                            {
                                for(int i=0;i<numoftickets;i++)
                                {
                                    // cout<<temp<<"=="<<numofseats[i]<<"\n";      
                                    if(numofseats[i]==temp)
                                    {
                                        temp="x";
                                        break;
                                    }        
                                }
                                // cout<<temp<<" ";
                                write_file << temp << " ";
                                temp = "";
                            }
                            else{
                                temp += line[i];
                            }
                        
                        }
                        write_file<<"\n";
                    }
                    read_file.close();
                    write_file.close();
                    remove(hallname);
                    rename("Garbage.txt",hallname);

                
            cout<<"\nCustomer Name: ";
            cin>>customername;
            cout<<"\nCustomer Phone: ";
            cin>>customerphone;
            BookTickets seeseats(hallname,moviename,numoftickets, numofseats,  customername,customerphone );   // parametrized constructor passing
            if(file.write((char*)&seeseats, sizeof(seeseats))){  // this is parametrized only it is filled when loop is runned
            cout<<"Booking Successful";
            }
            file.close();
        }
    }
    void bookedseatsdata()
    {
        BookTickets seedetails;
        ifstream file("Cinema_Seats_Booking.dat", ios::app); // read from binary file
        while (file.read((char *)&seedetails, sizeof(seedetails)))
        {
            seedetails.display();
        }
            file.close();
    }



};

int main(){
    
    
    system("cls"); // Use "clear" instead of "cls" on Unix-based systems
    for(;;){
    char option;
    cout<<"\nPRESS 1- SEE MOVIE DETAILS";
    cout<<"\nPRESS 2- CHECK SEATS AVAILABILITY";
    cout<<"\nPRESS 3- BOOK TICKETS";
    cout<<"\nEnter Option: ";
    cin>>option;
    if(option==50){
        system("cls"); // Use "clear" instead of "cls" on Unix-based systems
        char choice;
        CheckSeatsAvailability ObjectCheckSeatsAvailability;
        CheckSeatsAvailability_in_text_file ObjectCheckSeatsAvailability_in_text_file;

        cout<<"\n-------------------------------------------------------";
        cout<<"\nPress 1- Insert Seats";
        cout<<"\nPress 2- DISPLAY SEATS";
        cout<<"\n-------------------------------------------------------";
        cout<<"\nEnter Choice: ";
        cin>>choice;
        if(choice==49){
            system("cls"); // Use "clear" instead of "cls" on Unix-based systems
            ObjectCheckSeatsAvailability.insertingseats();
            ObjectCheckSeatsAvailability_in_text_file.insertseats();
        }
        else if(choice==50){
            system("cls"); 
            ObjectCheckSeatsAvailability_in_text_file.readingfromtextfile();
            // ObjectCheckSeatsAvailability.showseats();
        }
        else if(choice>=58 && choice<=125){
            cout<<"In Valid Choice";
            break;
        }
        else if(choice>=33 && choice<=48){
            cout<<"In Valid Choice";
            break;
        }

    }
    else if(option==49){
        system("cls"); 
        char choice;
        SeeMovieDetails ObjectSeeMovieDetails;
        cout<<"\n-------------------------------------------------------";
        cout<<"\nPress 1- INSERT MOVIES";
        cout<<"\nPress 2- DISPLAY MOVIES";
        cout<<"\nPress 3- UPDATE MOVIE Details";
        cout<<"\nPress 4- DELETE MOVIE Details";
        cout<<"\nPress 5- SEARCH MOVIE";
        cout<<"\n-------------------------------------------------------";
        cout<<"\nEnter Choice: ";
        cin>>choice;
        if(choice==49){

            system("cls"); 
            ObjectSeeMovieDetails.insertingmovie();
        }
        else if(choice==50){
            system("cls"); 
            ObjectSeeMovieDetails.showmovies();
        }
        else if(choice==51){
            system("cls"); 
            ObjectSeeMovieDetails.updatemovie();
        }
        else if(choice==52){
            system("cls"); 
            ObjectSeeMovieDetails.deletemovie();
        }
        else if(choice==53){
            system("cls"); 
            ObjectSeeMovieDetails.search();
        }
        else if(choice>=58 && choice<=125){
            system("cls"); 
            cout<<"In Valid Choice";
            break;
        }
        else if(choice>=33 && choice<=48){
            system("cls"); 
            cout<<"In Valid Choice";
            break;
        }

    }
    else if(option==51){
        system("cls"); 
        char choice;
        BookTickets ObjectBookTickets;
        cout<<"\n-------------------------------------------------------";
        cout<<"\nPress 1- Book Seats";
        cout<<"\nPress 2- Booked SEATS Data";
        cout<<"\n-------------------------------------------------------";
        cout<<"\nEnter Choice: ";
        cin>>choice;
        if(choice==49){
            system("cls"); 
            ObjectBookTickets.bookingseats();
        }
        else if(choice==50){
            system("cls"); 
            ObjectBookTickets.bookedseatsdata();
        }
        else if(choice>=58 && choice<=125){
            cout<<"In Valid Choice";
            break;
        }
        else if(choice>=33 && choice<=48){
            cout<<"In Valid Choice";
            break;
        }

    }
    
    else if(option>=58 && option<=125){
        cout<<"In Valid Choice";
        break;
    }
    else if(option>=33 && option<=48){
        cout<<"In Valid Choice";
        break;
    }
    
    
    }
}
