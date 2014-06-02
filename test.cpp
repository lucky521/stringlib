
#include <iostream>
#include <string>
#include <vector>
#include "stringlib.h"


using namespace std;

int main(int argc, const char * argv[])
{
    string s1 = "lucky";
    string s2 = "ck";
    cout<<stringlib::title(s1)<<endl;
    cout<<stringlib::strip(s1, " ")<<endl;
    cout<<stringlib::find(s1, s2)<<endl;
    cout<<stringlib::index(s1, s2)<<endl;
    
    string test_istitle = "abc aaa aBCc";
    cout<<stringlib::istitle(test_istitle)<<endl;
    cout<<stringlib::title(test_istitle)<<endl;
    
    
    string test_zfill = "+123";
    cout<<"***"<<stringlib::zfill(test_zfill, 10)<<"***"<<endl;
    cout<<"***"<<stringlib::center(test_zfill, 10)<<"***"<<endl;
    
    
    string test_mul = "test";
    cout<<stringlib::mul(s1, 3)<<endl;
    
    
    string test_splitlines = "first\nsecond\nthird";
    vector<string> test_splitlines_vector;
    stringlib::splitlines(test_splitlines, test_splitlines_vector);
    for(int i=0;i<test_splitlines_vector.size();i++)
        cout<<"Line: "<<i<<" "<<test_splitlines_vector[i]<<endl;;
    
    
    cout<<stringlib::replace("liulu", "lu", "ha")<<endl;
    
    return 0;
}

