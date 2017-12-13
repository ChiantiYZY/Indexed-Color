#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <utility>

using namespace std;

void createIndexedFile(int h, int w, int output[][4], vector<unsigned char> table) {
    char b;
    char m;
    int fileSize;
    short zero;
    int offsetToPixelData;
    int headerSize;
    int imageWidth;
    int imageHeight;
    short one;
    short bitsPerPixel;
    int compressionType;
    int compression;
    int preferredXResolution;
    int preferredYResolution;
    int numColorMapEntries;
    int numSignificantColors;
    unsigned char z=0;

    ofstream outFile("/home/yanz14/CLionProjects/lab1/output.bmp", ofstream::binary);
    b = 'B';
    outFile.write(reinterpret_cast<char *>(&b), 1);
    m = 'M';
    outFile.write(reinterpret_cast<char *>(&m), 1);
    fileSize = h * w;
    outFile.write(reinterpret_cast<char *>(&fileSize), 4);
    zero = 0;
    outFile.write(reinterpret_cast<char *>(&zero), 2);
    outFile.write(reinterpret_cast<char *>(&zero), 2);
    offsetToPixelData = 1078;
    outFile.write(reinterpret_cast<char *>(&offsetToPixelData), 4);
    headerSize = 40;
    outFile.write(reinterpret_cast<char *>(&headerSize), 4);
    imageWidth = w;
    outFile.write(reinterpret_cast<char *>(&imageWidth), 4);
    imageHeight = h;
    outFile.write(reinterpret_cast<char *>(&imageHeight), 4);
    one = 1;
    outFile.write(reinterpret_cast<char *>(&one), 2);
    bitsPerPixel = 8;
    outFile.write(reinterpret_cast<char *>(&bitsPerPixel), 2);
    compressionType = 0;
    outFile.write(reinterpret_cast<char *>(&compressionType), 4);
    compression = 0;
    outFile.write(reinterpret_cast<char *>(&compression), 4);
    preferredXResolution = 0;
    outFile.write(reinterpret_cast<char *>(&preferredXResolution), 4);
    preferredYResolution = 0;
    outFile.write(reinterpret_cast<char *>(&preferredYResolution), 4);
    numColorMapEntries = 256;
    outFile.write(reinterpret_cast<char *>(&numColorMapEntries), 4);
    numSignificantColors = 256;
    outFile.write(reinterpret_cast<char *>(&numSignificantColors), 4);

    for(int i = 0; i < 256; i ++) {
        outFile.write(reinterpret_cast<char *>(&output[i][0]), 1);
        outFile.write(reinterpret_cast<char *>(&output[i][1]), 1);
        outFile.write(reinterpret_cast<char *>(&output[i][2]), 1);
        outFile.write(reinterpret_cast<char *>(&output[i][3]), 1);
    }

    for(int i = 0; i < table.size() ; i++){
        outFile.write(reinterpret_cast<char *>(&(table.at(i))), 1);
    }
    outFile.close();
}

void swap(int *m, int *n)
{
    int temp = *m;
    *m = *n;
    *n = temp;

}

void selectionSort(vector<vector<int>> &arr, int n)
{
    for (int i = 0; i < n-1; i++)
    {
        // Find the maximum element in unsorted array
        int max = i;
        for (int j = i+1; j < n; j++) {
            if (arr.at(j).at(3) > arr.at(max).at(3)) {
                max = j;
            }
        }

        for(int m = 0; m < 5; m++)
        {
            swap(&arr.at(max).at(m),&arr.at(i).at(m));
        }

    }
}

void popularity(int array[][3],int width, int height)
{
    // 0/1/2 RGB 3 alpha
    int output[256][4];
    for(int i = 0; i < 256; i++)
    {
        output[i][0] = 0;
        output[i][1] = 0;
        output[i][2] = 0;
        output[i][3] = 1;
    }

    //check frequency
    vector<vector<int>> frequency;
    int count = 0;
    for(int i = 0; i < width*height; i++)
    {
        int flag = 0;
        for(int j = 0; j < frequency.size(); j++)
        {
            if(frequency.at(j).at(0) == array[i][0] && frequency.at(j).at(1) == array[i][1] && frequency.at(j).at(2) == array[i][2])
            {
                frequency.at(j).at(3)++;
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            count++;
            vector<int> RGB;
            RGB.push_back(array[i][0]);
            RGB.push_back(array[i][1]);
            RGB.push_back(array[i][2]);
            RGB.push_back(1);     //count
            RGB.push_back(i);     // original index
            frequency.push_back(RGB);
        }
    }

    cout << count ;
    //for(int i = 0; i < frequency.end())
    if(count < 256)
    {
        cout << "This picture only have " << frequency.size() << " colors, no need for compression.\n";
        exit(0);
    }

    selectionSort(frequency,frequency.size());


    for(int i = 0; i < 256; i++)
    {
        output[i][0] = frequency.at(i).at(0);
        output[i][1] = frequency.at(i).at(1);
        output[i][2] = frequency.at(i).at(2);
    }
    vector<unsigned char> table;
    int mapping[height*width];

    for(int i = 0; i < width*height; i++)
        mapping[i] = 0;


    for(int i=0; i < width*height; i++)
    {
        int min = (output[0][0] - array[i][0])*(output[0][0] - array[i][0]) + (output[0][1] - array[i][1])*(output[0][1] - array[i][1])+(output[0][2] - array[i][2])*(output[0][2] - array[i][2]);
        for(int j = 0; j < 256; j++)
        {
            int diff = (output[j][0] - array[i][0])*(output[j][0] - array[i][0]) + (output[j][1] - array[i][1])*(output[j][1] - array[i][1])+(output[j][2] - array[i][2])*(output[j][2] - array[i][2]);
            if(diff <= min)
            {
                min = diff;
                mapping[i] = j;
            }
        }
    }
//
    //change RGB to BGR so the picture could fit better
    for(int i = 0; i < 256; i++)
    {
        swap(&output[i][0],&output[i][2]);
    }

    for(int i = height-1; i >= 0; i--) {

        //store every pixel's index information into index Table
        for (int j = 0; j < width; j++) {
            int k = mapping[width*i + j];
            table.push_back((unsigned char)k);
        }

        //make sure width is multiply of 4
        if(width%4 != 0)
        {
            for (int k = 0; k < 4-width%4; k++) {
                table.push_back((unsigned char)0);
            }
        }

    }

    cout << "indexed color: " << endl;
    for(int i = 0; i < 256; i++)
    {
        cout << output[i][0] << " " << output[i][1] << " " <<output[i][2] << " " <<output[i][3] << " " <<endl;
    }

    createIndexedFile(height,width,output,table);

    return;

}

void uniform(int array[][3], int width, int height)
{
    // 0/1/2 BGR 3 alpha
    int output[256][4];

    //partition into 8*8*4 sections and mark each section with the leftest value
    // RGB = ijk
    for(int i = 0; i < 256;i++)
    {
        output[i][0] = 0;
        output[i][1] = 0;
        output[i][2] = 0;
        output[i][3] = 0;
    }

    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            for(int k = 0; k < 4; k ++){
                output[32*i + 4*j + k][0] = ((unsigned char) (rand()%64 + 64 * k)); //B
                output[32*i + 4*j + k][1] = ((unsigned char) (rand()%32 + 32 * j)); //G
                output[32*i + 4*j + k][2] = ((unsigned char) (rand()%32 + 32 * i)); //R
                output[32*i + 4*j + k][3] = ((unsigned char) 1);
            }
        }
    }

    //table stores the RGB partition index of all entries in the original table
    vector<unsigned char> table;
    int index = 0;
    for(int i = height-1; i >= 0; i --){
        for(int k = 0; k < width; k ++){
            //calculate the index of each pixel: R*4*8 + G*4 + B
            index = (unsigned char)array[i*width + k][0]/32*32 + (unsigned char)array[i*width + k][1]/32*4 + (unsigned char)array[i*width + k][2]/64;
            table.push_back((unsigned char) index);
        }
        //if width is not the multiple of 4, must add 0s into the table

        if(width%4 != 0)
        {
            for(int j = 0; j < 4-width%4; j ++){
                table.push_back((unsigned char) 0);
            }
        }
    }

    cout << "indexed color: " << endl;
    for(int i = 0; i < 256; i++)
    {
        cout << output[i][0] << " " << output[i][1] << " " <<output[i][2] << " " <<output[i][3] << " " <<endl;
    }


    createIndexedFile(height,width,output,table);
}


int main() {
    string filenameS;
    int height, width;
    long lSize;
    size_t result;
    FILE * infile;

    string address = "";
    cout << "what file you want to compress?\n";
    cin >> address;

    address = "/home/yanz14/CLionProjects/lab1/" + address + ".raw";

    char* tmp = new char[address.length() + 1];
    strcpy(tmp, address.c_str());

    cout << "What is the height?\n";
    cin >> height;
    cout << "What is the width?\n";
    cin >> width;

    infile = fopen (tmp, "rb" );
    if (infile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek(infile , 0 , SEEK_END);
    lSize = ftell (infile);
    rewind (infile);

    int array[width*height][3];
    for(int i = 0; i < width*height; i++)
    {
        array[i][0] = 0;
        array[i][1] = 0;
        array[i][2] = 0;
    }

    // allocate memory to contain the whole file:
    unsigned char * image = (unsigned char*) malloc (sizeof(unsigned char)*lSize);
    if (image == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the image
    result = fread (image,1,lSize,infile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    fclose(infile);


    //read the image into an 2D array
    int pix;
    for (int i = 0; i < height*width; i++) {
        array[i][0] = (int) (*(image + 3*i + 0));
        array[i][1] = (int) (*(image + 3*i + 1));
        array[i][2] = (int) (*(image + 3*i + 2));
        cout << array[i][0] << " " << array[i][1] << " "<<array[i][2] <<endl;
    }


    int **colorTable;
    char choice;
    cout << "choose popularity algorithm(p) or uniform algorithm(u):"<<endl;
    cin >> choice;

    if(choice == 'p')
    {
        popularity(array,width,height);
    }
    if(choice == 'u')
    {
        uniform(array,width,height);
    }

}