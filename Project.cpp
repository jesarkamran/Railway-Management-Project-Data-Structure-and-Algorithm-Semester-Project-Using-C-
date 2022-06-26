#include <iostream>
#include <sstream>
#include  <vector>
#include <stdexcept>  //For Invalid_Argument exception
#include  <string>
using namespace std;
struct Edge
{
    int index = -1;
    double distance;
    vector<string> trainArr;
};

struct Vertex
{
    int index = -1;
    string city_name;
};
struct Reserved
{
    string trainName;
    int seatNo;
    string name, phoneNo;
    bool reserved = false;
    Reserved *left = NULL, *right = NULL;
};
//Methods
void addEdge();
void autoInitializeVertexList();
void generateBill(Reserved *node);
void ticket_reservation();
void least_Cost();
void admin_Login();
void admin_page();
void add_vertex(Vertex p);
void mainMenu();
void displayTrains();
void dijkstra(string cities[],int size, int src);
void shortest_Distance();

            /*           GRAPH IMPLMENTATION         */

//Auto Edge Initialization
vector<vector<Edge*>> edges;

//NULL Initialization of Vector of Vectors
void initializeVectorOfVectors()
{

    for (int i = 0; i < 8; i++)
    {
        vector<Edge*> temp;
        for (int j = 0; j < 8; j++)
        {
            temp.push_back(NULL);
        }
        edges.push_back(temp);
    }
}
//Add Edge method which adds up edges
void add_Edge(int fromIndex, int toIndex, double distance, vector<string> arr)
{
    Edge *e = new Edge;
    e->distance = distance;
    e->trainArr = arr;
    edges[fromIndex][toIndex] = e;
    edges[toIndex][fromIndex] = e;
}
void autoInitializationOfEdges()
{
    initializeVectorOfVectors();
    Edge *t1, *t2, *t3;
    
    t1 = new Edge;
    t1->trainArr.push_back("Green Line");
    t1->trainArr.push_back("Tez Gum");
    t1->trainArr.push_back("Sir Syed");

    t2 = new Edge;
    t2->trainArr.push_back("Green Line");
    t2->trainArr.push_back("Tez Gum");

    t3 = new Edge;
    t3->trainArr.push_back("Green Line");


    
    add_Edge(0,1,150, t1->trainArr);
    add_Edge(0,2,300, t2->trainArr);
    add_Edge(0,6,800, t3->trainArr);
    add_Edge(1,5,500, t1->trainArr);
    add_Edge(1,6,650, t2->trainArr);
    add_Edge(1,3,150, t3->trainArr);
    add_Edge(4,3,100, t1->trainArr);
    add_Edge(7,3,700, t2->trainArr);
}
//Auto Edge Initialization Ends


//Auto Vertex Initialization
vector<Vertex> list(8);
string cityList[]  = {"Karachi", "Hyderabad", "Quetta",  "Peshawar", "Multan", "Faisalabad", "Lahore", "Rawalpindi"};

Vertex newVertexForAutoInitialize(int index, string city_name)
{
    Vertex curr;
    curr.index = index;
    curr.city_name = city_name;
    return curr;
}

void autoInitializeVertexList()
{
    for (int i = 0; i < list.size(); i++)
    {
        list[i] = newVertexForAutoInitialize(i, cityList[i]);
    }
    
}

//Auto Vertex Initialization Ends


            /*          END OF  GRAPH IMPLMENTATION         */



           /*           AVL Tree for Seats  Reservation         */
Reserved *rootReserved = NULL;

void InOrderDisplay(Reserved *p){
    if(p!=NULL){

        InOrderDisplay(p->left);
        
        cout<<"|\t"<<p->seatNo<<"\t";
        if (p->reserved == true)
        {
            cout<<"|  Reserved\t|"<<endl;
        }
        else
        {
            cout<<"|  Unreserved\t|"<<endl;
        }
        InOrderDisplay(p->right);
    }
}
void displaySeats()
{
    cout<<"\n---------------------------------"<<endl;
    cout<<"|\tTICKET DETAILS\t\t|"<<endl;
    cout<<"---------------------------------"<<endl;
    cout<<"|   TICKET NO   |  STATUS\t|"<<endl;
    cout<<"---------------------------------"<<endl;
    InOrderDisplay(rootReserved);
    cout<<"---------------------------------"<<endl;
}

int heightOfAVL(Reserved *p){
    if(p==NULL){
        return 0;
    }
    return max(heightOfAVL(p->left), heightOfAVL(p->right)) + 1;
}

int balanceFactor(Reserved *p){
    if (p==NULL){
        return -1;
    }else{
        return (heightOfAVL(p->left) - heightOfAVL(p->right));
    }
}


Reserved* rightRotate(Reserved *y) {

    Reserved* x = y -> left;
    Reserved* nullCheckerNode = x -> right;

    x -> right = y;
    y -> left = nullCheckerNode;

    return x;
}

Reserved* leftRotate(Reserved * x) {
    Reserved* y = x -> right;
    Reserved* nullCheckerNode = y -> left;

    y -> left = x;
    x -> right = nullCheckerNode;

    return y;
}
Reserved* insertReserve(Reserved *p,Reserved *newNode){
    if (p==NULL){
        p = newNode;
        return p;
    }else if(newNode->seatNo < p->seatNo){
        p->left = insertReserve(p->left,newNode);
    }else if(newNode->seatNo > p->seatNo){
        p->right = insertReserve(p->right,newNode);
    }else{
        cout<<"Can't Insert Duplicate  Values";
    }

    int bf = balanceFactor(p);

    //Checking for LL Rotations
    if(bf > 1 && newNode->seatNo < p->left->seatNo){
        return rightRotate(p);
    }
    //Checking for RR Rotations
    if(bf < -1 && newNode->seatNo > p->right->seatNo){
        return leftRotate(p);
    }
    //Checking for LR Rotations
    if(bf > 1 && newNode->seatNo > p->left->seatNo){
        p->left = leftRotate(p->left);
        return rightRotate(p);
    }
    //Checking for RL Rotations
    if(bf < -1 && newNode->seatNo < p->right->seatNo){
        p->right = rightRotate(p->right);
        return leftRotate(p);
    }
    //In the end I am going to return the pointer for the next recursive call
    return p;
}
void initializeReserved()
{
    for (int i = 1; i <= 10; i++)
    {
        Reserved *node = new Reserved;
        node->seatNo = i;
        rootReserved = insertReserve(rootReserved, node);
    }
}
Reserved* vacantSeat(Reserved *p, int desiredSeat)
{
    cout<<".";
    while (p!=NULL)
    {
        if (p->seatNo == desiredSeat)
        {
            if (p->reserved == false)
            {
                return p;
            }
            else
            {
                return NULL;
            }
        }
        else if (p->seatNo < desiredSeat)
        {
            p = p->right;
        }
        else
        {
            p = p->left;
        }
    }
    return NULL;

}
        /*          End of AVL Implementation           */


void generateBill(Reserved *node, double charges)
{
    cout<<"\n--------------------------------------------------------------------------"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TICKET<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    cout<<"--------------------------------------------------------------------------"<<endl;
    cout<<"|\t\tPassenger Name___________________"<<node->name<<"\t\t |"<<endl;
    cout<<"--------------------------------------------------------------------------"<<endl;
    cout<<"|\t\tPassenger PhoneNo________________"<<node->phoneNo<<"\t\t |"<<endl;
    cout<<"--------------------------------------------------------------------------"<<endl;
    cout<<"|\t\tSeat Number______________________"<<node->seatNo<<"\t\t\t |"<<endl;
    cout<<"--------------------------------------------------------------------------"<<endl;
    cout<<"|\t\tTicket Charges___________________"<<charges<<"\t\t\t |"<<endl;
    cout<<"--------------------------------------------------------------------------"<<endl;
}


//Checks Wether Train Goes from Departure Position to Destination Position
int indexFind(string cityName)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].city_name == cityName)
        {
            return i;
        }
    }
    return -1;
}
double trainGoes(string trainName, string departCity, string destCity)
{
    int index1 = indexFind(departCity);
    int index2 = indexFind(destCity);
    if (index2 != -1 && index1 != -1)
    {
        vector<string> tempList = edges[index1][index2]->trainArr;
        double distance = edges[index1][index2]->distance;
        for (string name: tempList)
        {
            if (name == trainName)
            {
                return distance;
            }   
        }
        return 0;
    }
    else
    {
        return 0;
    }   
}
void reservationDataGetter(Reserved *getNode)
{
    cout<<"\nEnter Departure City:"; string depart; cin>>depart;
    cout<<"Enter Destination City:"; string dest; cin>>dest;
    cout<<"Enter Train Name: "; string train; cin>>train;
    string temp = ""; cin>>temp; train = train+" "+temp;
    double charges = trainGoes(train, depart, dest)*3;
    cout<<"1Error to yaha pe hai hee nhn"<<endl;
    if (charges != 0)
    {
        getNode->trainName = train;
        cout<<"Error to yaha pe hai hee nhn"<<endl;
        string name1 = "", name2 = "";
        cout<<"Enter Passenger First Name: "; cin>>name1;
        cout<<"Enter Passenger Last Name: "; cin>>name2;
        
        getNode->name = name1+" "+name2;
        cout<<"\nEnter Passenger Contact No: "; cin>>getNode->phoneNo;
        getNode->reserved = true;
        
        generateBill(getNode, charges);
    }
    else
    {
        cout<<"Train does not go through DepartCity to DestCity"<<endl;
    }
}
//Ticket Reservation Method
void ticket_reservation()
{
    cout<<"\n"<<"---------------WELCOME TO-------------------------------------------------"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>TICKET RESERVATION<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    cout<<"--------------------------------------------------------------------------\n"<<endl;
    
    cout<<"Enter Seat Number you want to Reserve: "; int seatNumber; cin>>seatNumber;
    cout<<"\nProcessing";
    Reserved *getNode = new Reserved;
    getNode = vacantSeat(rootReserved,seatNumber);
    if (getNode != NULL)
    {
        reservationDataGetter(getNode);
    }
    else
    {
        cout<<"GOING BACK TO MAIN MENU"<<endl;
        cout<<"Maybe Tickets are Reserved or Reserved Seat No: again Entered\n"<<endl;
    
    }
}
void displayVertex()
{
    for (int i = 0; i < list.size(); i++)
    {
        cout<<"City Name: "<<list[i].city_name<<endl;
        cout<<"City Id: "<<list[i].index<<"\n"<<endl;
    }
}
Vertex newVertex(int index)
{
    Vertex curr;
    curr.index = index;
    cout<<"Enter City Name: ";
    cin>>curr.city_name;
    return curr;
}
bool checkVertexExists(int index)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).index == index)
        {
            return true;
        }
    }
    return false;   
}
void add_vertex(Vertex p)
{
    if (checkVertexExists(p.index))
    {
        cout<<"Vertex Exists"<<endl;
    }
    else
    {
        list.push_back(p);
    }
}
void addEdge()
{
    cout<<"Enter City ID: "; int city1; cin>>city1;
    cout<<"Enter City ID: "; int city2; cin>>city2;
    cout<<"Enter Distance: "; double dist; cin>>dist;
    cout<<"Enter Number of Train:"; int num; cin>>num;
    vector<string> train(num);
    for (int i = 0; i < train.size(); i++)
    {
        cout<<"Enter train Name: "; string train1, train2; cin>>train1; cin>>train2;
        train[i] = train1+" "+train2;
    }
    
    if (city1 < list.size()&& city2 <list.size())
    {
        add_Edge(city1, city2,dist,train);
    }
    else
    {
        cout<<"Invalid Ids"<<endl;
        addEdge();
    }
    
    
}
void admin_page()
{
    cout<<"\n1- ADD CITY"<<endl
        <<"2- ADD PATH"<<endl
        <<"3- BACK TO MainMenu"<<endl;
    cout<<"\nEnter your choice: "; int choice; cin>>choice;
    switch (choice)
    {
        case 1:
            add_vertex(newVertex(list.size())); displayVertex(); admin_page(); break;
        
        case 2:
            addEdge(); break;
        
        case 3:
            break;
        
        default:
            cout<<"Invalid Choice Entered Try Again!!"<<endl;
            admin_page(); break;
    }
}
void admin_Login()
{
    cout<<"\n"<<"---------------WELCOME TO-------------------------------------------------"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>ADMIN LOGIN<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    cout<<"--------------------------------------------------------------------------\n"<<endl;
    string admin = "admin", password = "admin";
    cout<<"Enter Admin Name: "; string str1; cin>>str1;
    cout<<"Enter Admin Password: "; string str2; cin>>str2;
    if (str1 == admin && str2 == password)
    {
        admin_page();
    }
    else
    {
        cout<<"Incorrect Admin name or Password Entered Try Again!!!\n"<<endl;
        admin_Login();
    }
}
void displayTrains(string trainName)
{
    vector<string> detailSave;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (edges[i][j] != NULL)
            {
                vector<string> temp = edges[i][j]->trainArr;
                for (int k = 0; k < temp.size(); k++)
                {
                    if (trainName == temp[k])
                    {
                        
                        ostringstream os;
                        os<<edges[i][j]->distance;
                        string con = os.str();
                        string data;
                        if (list[i].city_name.length() > 8)
                        {
                            data = "From: "+list[i].city_name+" |To: "+list[j].city_name+"\t|Distance: "+con+" km";
                            
                        }
                        else
                        {
                            data = "From: "+list[i].city_name+"\t |To: "+list[j].city_name+"\t|Distance: "+con+" km";
                        }
                        
                        detailSave.push_back(data);
                    }   
                }
            }
        }
    }
    cout<<"--------------------------------------------------"<<endl;
    if (trainName.length() >7)
    {
        cout<<"|  TRAIN  \t|\t"<<trainName<<"\t\t |"<<endl;
    }
    else
    {
        cout<<"|  TRAIN  \t|\t"<<trainName<<"\t\t\t |"<<endl;
    }
    cout<<"--------------------------------------------------"<<endl;
    cout<<"|  Departure  \t|  Destination \t|  Distance  \t |"<<endl;
    cout<<"--------------------------------------------------"<<endl;
    for(int i = 0; i < detailSave.size(); i++)
    {
        cout<<"|"<<detailSave[i]<<"|"<<endl;
    }
    cout<<"--------------------------------------------------"<<endl;

    cout<<"\n";
}

vector<string> trains = {"Green Line", "Tez Gum", "Sir Syed"};
void display()
{
    for (int i = 0; i < trains.size(); i++)
    {
        displayTrains(trains[i]);            
    }
}
//Feedback Using Doubly Linked List
struct Feedback
{
    int indexOfTrain;
    string feedbackStars;
    string name;
    int age;
    int indexUserCity;

    Feedback *prev= NULL, *next = NULL;
};
Feedback *first= NULL, *last = NULL;
int indexFindTrain(string trainName, vector<string> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i] == trainName)
        {
            return i;
        }
    }
    return-1;
}
void insert_End(Feedback *curr)
{
    if (first == NULL)
    {
        first = last = curr;
    }
    else
    {
        last->next = curr; curr->prev = last;last = curr;
    }
}
void feedback()
{
    cout<<"\n_-_-__FEEDBACK___-_-_\n"<<endl;
    cout<<"\nEnter Your Full Name: ";string name, name2; cin>>name;
    cin>>name2; name = name +" "+ name2;

    cout<<"Enter your Age: "; int age; cin>>age;
    cout<<"Enter Your City of Travel Name: "; string cityName; cin>>cityName;

    cout<<"Enter Train Name: "; string train, temp; cin>>train; cin>>temp;
    train = train+" "+temp;

    cout<<"Give your Feedback from *****: "; string feed; cin>>feed;
    int indextrain = indexFindTrain(train, trains);
    int indexCity = indexFind(cityName);
    if (indexCity !=-1 && indextrain != -1)
    {

        cout<<"Something is getting saved"<<endl;
        Feedback *curr = new Feedback;
        curr->name = name; curr->age = age;
        curr->feedbackStars = feed; curr->indexUserCity = indexCity;
        curr->indexOfTrain = indextrain;
        insert_End(curr);
    }
        
}
void displayFeedback()
{
    if (first != NULL)
    {
        Feedback *p = first;
        cout<<"\n";
        cout<<"SHOWING ALL FEEDBACKS OF USERS\n"<<endl;
        while (p!=NULL)
        {
            cout<<"User Name: "<<p->name<<endl;
            cout<<"User Age: "<<p->age<<endl;
            cout<<"User City of Travel: "<<list[p->indexUserCity].city_name<<endl;
            cout<<"Train Name: "<<trains[p->indexOfTrain]<<endl;
            cout<<"Number of Stars Given: "<<p->feedbackStars<<endl;
            cout<<"\n";
            p = p->next;
        }
    }
    else
    {
        cout<<"No Data to Display";
    }
}
void shortest_Distance()
{
    int size = edges.size();
    string cities[size];
    for (int i = 0; i < size; i++)
    {
        cities[i] = list[i].city_name;
    }
    
    cout<<"Select City from there you want to know shortest distance of Other cities "<<endl;
    for (int i = 0; i < size; i++)
    {
        cout<<cities[i]<<endl;
    }
    string city;
    cout<<"Enter City: "; cin>>city;
    
    // Finds Index of Source city
    int index = -1;
    for(int i = 0; i<size; i++)
  {
    if(city== cities[i])
    {
      index = i;
      break;
    }
  }

    if (index != -1)
    {
        dijkstra(cities,size, index);
    }
    else
    {
        cout<<"Given City not Found!!!"<<endl;
    }
}

  
void dijkstra(string cities[], int size, int src)
{
    int graph[size][size];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (edges[i][j] != 0)
            {
                graph[i][j] = edges[i][j]->distance;
            }
            else
            {
                graph[i][j] = 3000;
            }
        }
    }
    
    int count = 1;
    
    int path[size];
    for(int i=0;i<size;i++)
        path[i] = graph[src][i];
    
    int visited[size] = {0};
    visited[src] = 1;
    
    while(count<size)
    {
        int minNode;
        int minVal = 3000;
        
        for(int i=0;i<size;i++)
            if(visited[i] == 0 && path[i]<minVal)
            {
                minVal = path[i];
                minNode = i;
            }
        
        visited[minNode] = 1;
        
        for(int i=0;i<size;i++)
            if(visited[i] == 0)
                path[i] = min(path[i],minVal+graph[minNode][i]);
                    
        count++;
    }
    
    path[src] = 0;
    cout<<"\n"<<endl;
    for(int i=0;i<size;i++)
        cout<<"Shortest distance from "<<cities[src]<<" to "<<cities[i]<<" -> "<<path[i]<<endl;
}
int isInt(string variable)
{
    try
    {
        int num = stoi(variable);
        if (num >0 and num < 9)
        {
            return num;
        }
    }
    catch(const exception& e)
    {

    }
    return 0;
}
void mainMenu()
{
    cout<<"\n"<<"---------------WELCOME TO--------------------------------------------------"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>RAILWAY MANAGEMENT SYSTEM<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    cout<<"---------------------------------------------------------------------------\n"<<endl;

    cout<<"\n1- RESERVE A TICKET"<<endl<<
        "2- ADMIN LOGIN"<<endl<<
        "3- DISPLAY DETAILS OF SEATS"<<endl<<
        "4- DISPLAY DETAILS OF TRAINS"<<endl<<
        "5- FIND SHORTEST DISTANCE FROM A CITY TO ALL CITIES"<<endl<<
        "6- GIVE FEEDBACK"<<endl<<
        "7- DISPLAY ALL FEEDBACKs"<<endl<<
        "8- EXIT"<<endl;

    cout<<"Chose Option No: "; string choice; cin>>choice;
    switch (isInt(choice))
    {

    case 1:
        ticket_reservation(); mainMenu(); break;

    case 2:
        admin_Login(); mainMenu(); break;
    
    case 3:
        displaySeats(); mainMenu(); break;

    case 4:
        display(); mainMenu(); break;
    
    case 5:
        shortest_Distance(); mainMenu(); break;
    
    case 6:
        feedback(); mainMenu(); break;
    
    case 7:
        displayFeedback(); mainMenu(); break;
    
    case 8:
        cout<<"\nProcess Finished Successfully\n"<<endl;
        break;
    
    case 0:
        cout<<"\nCan't Enter string Values\n"<<endl;
        mainMenu(); break;
    
    default:
        cout<<"Invalid Choice Entered Try Again!!"<<endl;
        mainMenu(); break;
    }    
}
int main()
{
    //Calling methods which Auto Initialize Data 
    initializeReserved();
    autoInitializeVertexList();
    autoInitializationOfEdges();

    //After Initializing Necessary Data 
    //Showing user Main Menu
    mainMenu();
    return 0;
}
