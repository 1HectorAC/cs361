
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std;

//Global variable. number of values in the file
int size = 10000000;

void merge(int *A, int p, int q, int r){
    //size of first half of array
    int n1 = q - p + 1;
    //size of second half of array
    int n2 =  r - q;
    //arrays are dynamics so that they can be variable lenght
    int* L = new int[n1 + 1];
    int* R = new int[n2 + 1];

    //set up L array with left side items
    for (int i = 1; i <= n1; i++)
        L[i] = A[p + i - 1];

    //setup R array with right side items
    for(int j = 1; j <= n2; j++)
        R[j] = A[q+j];

  int i = 1;
  int j = 1;
    for(int k = p; k <= r; k++){
        //this is called when we hit the end of the left array so we have to add from right array
        if(n1< i ){
            A[k] = R[j];
            j++;
        }
        //this is called when we hit the end of the right array so we have to add from left array
        else if(n2< j ){
            A[k] = L[i];
            i++;
        }
        //this is called when left array has the smaller value so its value is added to the main array
        else if(L[i] <= R[j]){
            A[k] = L[i];
            i = i + 1;
        }
        //this is called when right array has the smaller value so its value is added to the main array
        else{
            A[k] = R[j];
            j = j + 1;
        }

    }
    //L and R arent needed anymore so delete to free memory
    delete[] L;
    delete[] R;

}

void auxMergeSort(int *array, int p, int r){
    //exit when start index (p) is larger than end index (r)
    if(p < r){
        //this calculates the center index value
        int q = (int)(floor((p+r)/2));
        //splits array from start to center index value
        auxMergeSort(array, p, q);
        //splits array from center index value + 1 to end
        auxMergeSort(array,q+1, r);
        //combine arrays
        merge(array, p, q , r);

    }
}

int partition(int *A, int p, int r){
    //ends endless loop of a list of two equal values resulting from constantly return of second numbers index
    if(r == p + 1 && A[p] == A[r]){
        return p;
    }
    //mid point index
    int m = (int)floor((p + r) / 2);
    //average of start, end, and mid values in array. Also the pivot point
    int x = (int)floor((A[p]+A[r]+A[m]) / 3);

    int i = p - 1;

    for(int j = p; j <= r; j++){
        //check if value in array at location j is lower than the pivot point in order to swap it to the left
        if(A[j] <= x){
            i = i + 1;
            //exchange A[j] and A[i]
            int temp = A[j];
            A[j] = A[i];
            A[i] = temp;
        }
    }
    //return index largest number lower than pivot
    return i;
}

void auxQuickSort(int *A, int p, int r){
    //exit when start index (p) is larger than end index (r)
    if(p < r){
        //sort array with pivot
        int q = partition(A, p, r);
        //split array from start to q
        auxQuickSort(A, p, q);
        //split array from q+1 to end
        auxQuickSort(A, q+1, r);
    }
}


int flgIsSortedHelper(int *A, int p, int r){
    //exit when start index (p) is larger than end index (r)
    if(p < r){
        //get center of array index
        int q = (int)floor(((p+r)/2));
        //split array into half from start  to half(q)
        int one = flgIsSortedHelper(A, p, q);
        //split array into half from half(q) + 1 to end
        int two =  flgIsSortedHelper(A,q+1, r);
        //compares the inbetween points (q and q+1) of two  halfs of sorted array and returns 0 if sorted or 1 if not
        if(A[q] <= A[q+1]){
            return 0 + one + two;
        }
        else
            return 1 + one + two;
            //any instance of 1 or falseness will be added to everycheck along the way so it is not considered sorted if this section is ever called
    }
    //returns zero here when array is just one number long
    return 0;
}

bool flgIsSorted(int *A){
   //size of array or end index to check array
    //check will be equal to zero if true and greater than 0 if false
    int check = flgIsSortedHelper(A, 0, size);
    //return true or false depending on check
    if(check == 0)
        return true;
    else
        return false;
}

int* makeArrayFromFile(string file){
    ifstream files;
    files.open(file);

    //check if file exists
    if ( files.fail())
        cout << "file doesnt exits" << endl;

    //create array
    int* data = new int[size];

    //set up array with data from file
    for (int x = 0; x < size; x++){
       files >> data[x];
    }
    files.close();
    return data;
}

int main()
{

    auto start1 = std::chrono::high_resolution_clock::now();
    //create array
    int* data = makeArrayFromFile("lab1_data.txt");

   auto finish1 = std::chrono::high_resolution_clock::now();
   cout << "Time For Setting Up Array: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish1-start1).count() << "ns\n";

    //create sum of all data from file and output to console
    long long sum = 0;
    for (int x = 0; x < size; x++)
       sum += data[x];
    cout << "Sum of Values In Array: "<< sum << endl;

    //Merge sort run part
    auto start2 = std::chrono::high_resolution_clock::now();
    auxMergeSort(data, 0, size);
    auto finish2 = std::chrono::high_resolution_clock::now();
    cout << "Time for MergeSort to Run: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish2-start2).count() << "ns\n";

    int* data2 = makeArrayFromFile("lab1_data.txt");
    auto start3 = std::chrono::high_resolution_clock::now();
   auxQuickSort(data2, 0, size);
    auto finish3 = std::chrono::high_resolution_clock::now();
    cout << "Time for QuickSort to Run: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish3-start3).count() << "ns\n";

    cout << "Merge Sort Success Check: ";
    auto start4 = std::chrono::high_resolution_clock::now();
     if(flgIsSorted(data) == true)
        cout << "Success" << endl;
     else
        cout << "Fail" << endl;
    auto finish4 = std::chrono::high_resolution_clock::now();
     cout << "Time for Running Check on Merge Sort: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(finish4-start4).count() << "ns\n";

     //Quick sort run part
     cout<< "Quck sort success check: ";
     auto start5 = std::chrono::high_resolution_clock::now();
      if(flgIsSorted(data2) == true)
         cout << "Success" << endl;
      else
         cout << "Fail" << endl;
     auto finish5 = std::chrono::high_resolution_clock::now();
      cout << "Time for Running Check on Quick Sort: " <<  std::chrono::duration_cast<std::chrono::nanoseconds>(finish5-start5).count() << "ns\n";

     delete [] data;
     delete [] data2;

}
