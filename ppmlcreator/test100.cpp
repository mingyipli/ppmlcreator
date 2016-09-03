#include <iostream>
#include <fstream>
#include <string>
#include <math.h> 
#include <algorithm> 
#include <cstdio>
#include <ctime>

using namespace std;

//hardcode trimbox

double dpi = 812.; //mark background.tif and page background.tif dpi
double trimbox_x = 7993./dpi * 72.;
double trimbox_y = 5146./dpi * 6.  * 72.;
double page_x = 7993./dpi * 72.;
double page_y = 5146./dpi * 72.;

//page background.tif info
string page_background = "Crop.tif";
string ref_page_background = "A";
double page_background_x = 7993./dpi * 72.;
double page_background_y = 4958./dpi * 72.;

//mark background.tif info
string mark_background = "Eaoron.tif";
string ref_mark_background = "B";
double mark_background_x = 707./dpi * 72.; //704
double mark_background_y = 480./dpi * 72.; //377

//number of records being printed
int mark_number = 600*1;
int page_number;

//mathematics
double margin_x = (page_x - 10. * mark_background_x)/2.;
double margin_y = (page_y -10. * mark_background_y)/2;



void reusable_object(ofstream &obj, double x, double y, double width, double height, string pic, string ref) {
	obj << "<REUSABLE_OBJECT>\n";
	obj << "<OBJECT Position=\"" + to_string(x) + " " + to_string(y) + "\">\n";
	obj << "<SOURCE Format=\"image/tiff\" Dimensions=\"" + to_string(width) + " " + to_string(height) + "\">\n";
	obj << "<EXTERNAL_DATA Src=\"" + pic + "\"/>\n";
	obj << "</SOURCE>\n";
	obj << "<VIEW/>\n";
	obj << "</OBJECT>\n";
	obj << "<VIEW/>\n";
	obj << "<OCCURRENCE_LIST>\n";
	obj << "<OCCURRENCE Name=\""+ ref +"\">\n";
	obj << "<VIEW/>\n";
	obj << "</OCCURRENCE>\n";
	obj << "</OCCURRENCE_LIST>\n";
	obj << "</REUSABLE_OBJECT>\n";
}

void object(ofstream &obj, double x, double y, double width, double height, string location) {
	obj << "<OBJECT Position=\"" + to_string(x) + " " + to_string(y) + "\">" << endl;
	obj << "<SOURCE Format=\"image/tiff\" Dimensions=\"" + to_string(width) + " " + to_string(height) + "\">" << endl;
	obj << "<EXTERNAL_DATA Src=\"" + location + "\"/>" << endl;
	obj << "</SOURCE>" << endl;
	obj << "<VIEW/>" << endl;
	obj << "</OBJECT>" << endl;
}

int main()
{
    std::clock_t start;
    double duration;
    start = std::clock();
    
    if (mark_number % 600 == 0) {
		page_number = mark_number / 600;
	}
	else page_number = mark_number / 600 + 1;
    
	
	ifstream data("mapping.csv");
	string properties;
	getline(data, properties, ',');
    cout << properties << endl;
    
    ofstream myfile;
	myfile.open("EA.ppml");
	myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	myfile << "<!DOCTYPE PPML PUBLIC \"-//PODi//DTD PPML 2.1//EN\" \"http://www.podi.org/ppml/ppml210.dtd\">\n";
	myfile << "<PPML>\n";
    
	//define trimbox
	myfile << "<PAGE_DESIGN TrimBox=\"0 0 "+ to_string(trimbox_x)+" "+ to_string(trimbox_y) +"\"/>\n";
	//job level
	myfile << "<JOB Label=\"1\">\n";
    
	//deine reusable_object
	reusable_object(myfile, 0, 0, page_background_x, page_background_y, page_background, ref_page_background);
	reusable_object(myfile, 0, 0, mark_background_x , mark_background_y , mark_background, ref_mark_background);
    
	//document level
	myfile << "<DOCUMENT Label=\"1\">" << endl;
	
	//page level
	for (int i = 1 ; i <= page_number; i++) {
		myfile << "<PAGE Label=\"" + to_string(i) + "\">" << endl;
		
		//loop sub page
		for (int j = 0; j < 6; j++){
			myfile << "<MARK Position=\"0.0 " + to_string(page_y *(6 - 1 - j)) + "\">" << endl;
		    myfile << "<OCCURRENCE_REF Ref=\"" + ref_page_background + "\"/>" << endl;
			myfile << "</MARK>" << endl;

			//loop mark in sub page
			for (int k_y = 0; k_y < 10; k_y++) {
				for (int k_x = 0; k_x < 10; k_x++) {
					myfile << "<MARK Position=\"" + to_string(margin_x + k_x * mark_background_x ) + " " + to_string(margin_y + (10 - 1 - k_y)* mark_background_y + page_y *(6 - 1 - j)) + "\">" << endl;
					myfile << "<OCCURRENCE_REF Ref=\"" + ref_mark_background + "\"/>" << endl;
					myfile << "</MARK>" << endl;
					//string value;
					//getline(data, value, ',');
				}
			}
		}
		myfile << "</PAGE>" << endl;
	};
	
	myfile << "</DOCUMENT>" <<endl;
	myfile << "</JOB>\n";
	myfile << "</PPML>\n";
	myfile.close();
    
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "prosessing time needed for "<<mark_number<< " marks: "<<  duration  <<'\n';
     cout << "prosessing time needed for 1000000"<< " marks: "<<  1000000./mark_number *duration  <<'\n';


	return 0;

}