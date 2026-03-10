/*
 * ============================================================
 *  Arctic Research Station Management System
 *  INFO1102 — Principles of Programming II
 *  Author : Jovial Elyas Bearivo
 * ============================================================
 *
 *  NOTE: A video walkthrough of this project is available at:
 *  https://youtu.be/cPW7VWs17TM?si=CSdxppgfbEro8_tW
 *
 *  ⚠️  The video is in FRENCH, as this project was submitted
 *  in a francophone academic context (Université de Moncton, NB).
 *  The explanation covers the following design decisions:
 *
 *  1. BUBBLE SORT
 *     Chosen for its simplicity and readability over a fixed-size
 *     array of 5 resources. O(n²) is perfectly acceptable at this
 *     scale, and it demonstrates loop-based sorting logic clearly.
 *
 *  2. ENUM (TypeRessource)
 *     Used to categorize resources as VITAL, TECHNIQUE, or
 *     CONSOMMABLE. Enums improve code readability and prevent
 *     the use of arbitrary integer "magic numbers".
 *
 *  3. STRUCT & UNION (RessourceJB / SpecInfo)
 *     A struct groups all attributes of a resource into a single
 *     logical unit. The union (SpecInfo) stores either a pressure
 *     (int) or a temperature (double) — never both — saving memory
 *     when only one spec applies depending on the resource type.
 *
 *  4. FILE MANIPULATION
 *     ifstream reads the initial resource data from database1.txt.
 *     ofstream with ios::app appends every operation to
 *     journal_bord.txt, creating a persistent operation log
 *     that survives across program runs.
 *
 * ============================================================
 */
#include <iostream>
#include <fstream> //library for file manipulation
using namespace std;

//enumeration to represent the different types of resources
enum ResourceType { VITAL = 0, TECHNICAL = 1, CONSUMABLE = 2};

//union allowing to store either a pressure or a temperature (one value at a time)
union SpecInfo {
    int pressure;       //pressure value (used for certain resources)
    double temperature; //temperature value (used for technical resources)
};

//structure representing a complete resource
struct ResourceJB {
    string name;        //name of the resource
    int quantity;       //available quantity
    ResourceType type;  //type of the resource (Vital, Technical or Consumable)
    SpecInfo spec;      //specific information (pressure or temperature)
};

const int NB_RESOURCES = 5; //fixed number of resources in the array

//function that records the different operations performed by the user into a log file
void recordLog(string message){
    ofstream file("journal_bord.txt", ios::app); //opens the file without overwriting existing data
    if(!file.is_open()) //check if the file opened successfully
        return;
    file << "[LOG] " << message << endl; //writes the message into the file
    file.close(); //close the file
}

//function that opens our database file and processes the available resources
void loadData(ResourceJB* array, string fileName){
    ifstream file(fileName); //opens the file in read-only mode
    if(!file.is_open()) //check that the file opened successfully
        throw string("Error! The file could not be opened.");

    int i = 0;          //index to fill the array
    int typeInt;        //temporary variable to read the type as an integer
    double specVal;     //temporary variable to read the specific value

    //chaining of extraction operators
    while(file >> array[i].name >> array[i].quantity >> typeInt >> specVal){
        array[i].type = (ResourceType)typeInt; //convert the integer to ResourceType

        if(array[i].type == TECHNICAL) //if the resource is of technical type
            array[i].spec.temperature = specVal; //assign the temperature
        else
            array[i].spec.pressure = (int)specVal; //otherwise assign the pressure
        i++; //move to the next resource
    }
    file.close(); //close the file
    recordLog("Data loaded successfully."); //update our log
}

//function that updates the stock of a resource after the user consumes it
void updateStock(ResourceJB* resource, int quantityToConsume){
    if((resource->quantity - quantityToConsume) < 0) //check if the remaining quantity would be negative
        throw string("Insufficient stock!"); //throw an exception if stock is insufficient
    else{
        resource->quantity -= quantityToConsume; //subtract the consumed quantity from the stock
        string message = "Consumed " + to_string(quantityToConsume) + " units of " + resource->name;
        recordLog(message); //record the action in the log
    }
}

//function that displays the current state of available resources
void displayResources(ResourceJB* array, int size){
    cout << "NAME\t\tQTY\tTYPE\tSPEC" << endl; //display the table header
    for(int i = 0; i < size; i++){ //iterate through all resources
        cout << array[i].name << "\t\t" << array[i].quantity << "\t"; //display name and quantity
        if(array[i].type == TECHNICAL) //if type is technical
            cout << "Temp: " << array[i].spec.temperature; //display the temperature
        else
            cout << "Pres: " << array[i].spec.pressure; //otherwise display the pressure

        cout << endl; //new line
    }
    recordLog("Resources displayed."); //record the action in our log
}

//function that uses bubble sort to sort resources — O(n^2) complexity
void sortResources(ResourceJB* array, int size){
    for(int i = 0; i < size - 1; i++){ //outer loop for the number of passes
        for(int j = 0; j < size - i - 1; j++){ //compare adjacent elements
            if(array[j].quantity > array[j+1].quantity){ //if current element is greater than the next
                //swap the two resources
                ResourceJB temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
    recordLog("Inventory sorted by quantity."); //log that the sort was performed
}

int main(){
    ResourceJB array[NB_RESOURCES]; //declare the resource array
    int choice; //variable to store the menu choice

    try{
        loadData(array, "database1.txt"); //load data from the file
    } catch(string e){ //catch an exception of type string
        cerr << e << endl; //display the error in the error output
        return 1; //exit the program if errors exist
    }

    do{
        cout << "\n============================================\n\n";
        cout << "--- ARCTIC STATION : MANAGEMENT MENU ---" << endl;
        cout << "1. Display resource status" << endl;
        cout << "2. Consume a resource" << endl;
        cout << "3. Sort by quantity (Ascending)" << endl;
        cout << "4. Quit" << endl;
        cout << "Your choice : ";
        cin >> choice; //read the user's choice
        cout << "\n============================================\n";

        //check if the input is invalid
        if(cin.fail()){
            cin.clear(); //reset the error state
            cin.ignore(1000, '\n'); //flush the input buffer
            recordLog("Invalid option selected."); //record the action in the log
            cout << "\nInvalid option!" << endl;
            continue; //restart the loop
        }

        //process the choice with a switch
        switch(choice){
            case 1:{
                displayResources(array, NB_RESOURCES); //call the display function
                break;
            }
            case 2: {
                int index, quantity; //variables to select the resource and quantity
                cout << "Resource index (0-4) : ";
                cin >> index; //read the chosen index
                cout << "Quantity to consume : ";
                cin >> quantity; //read the quantity to consume
                try {
                    if(index < 0 || index >= NB_RESOURCES) //check if the index is valid
                        throw string("Invalid index!"); //exception if invalid
                    updateStock(&array[index], quantity); //otherwise update the stock
                    cout << "\nUpdate completed." << endl;
                } catch(string e){
                    cerr << e << endl; //display error if one occurs
                    recordLog("Error: " + e); //log the error
                }
                break;
            }
            case 3:{
                sortResources(array, NB_RESOURCES); //sort the resources
                displayResources(array, NB_RESOURCES); //display after sorting
                break;
            }
            case 4:{
                cout << "\nShutting down system..." << endl; //shutdown message
                break;
            }
            default:{
                recordLog("Invalid option selected."); //if chosen option is invalid, log it
                cout << "\nInvalid option!" << endl;
                break;
            }
        }
    }while(choice != 4); //the loop continues as long as the user does not choose 4

    return 0; //end of program, thanks for reading my code! thnak you so much 
}
