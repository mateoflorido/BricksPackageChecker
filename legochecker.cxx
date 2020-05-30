#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <map>

using namespace cv;
using namespace std;
const Point neighbors[8] = {Point(1, 0), Point(1, -1), Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0, 1),
                            Point(0, -1), Point(1, 1)};
const
string DetectShape(vector<Point> &shape, float cx, float cy) {
    //Check perimeter of closed Shape
    double perimeter = arcLength(shape, true);
    //Approximate shape for better recognition
    vector<Point> approx;
    approxPolyDP(shape, approx, 0.015 * perimeter, true);
    float area = contourArea(shape);
    if (perimeter > 80) {
        if (approx.size() == 4) {
            Rect r = boundingRect(approx);
            float ar = (float) r.width / (float) r.height;
            if (area > 23500 && area < 27000) {
                std::cout << "Se detectó como: P. 2x4 con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size()
                          << " AR:" << ar << " Area: " << area << std::endl;
                return "P. 2x4";
            }else if (area > 5000 && area < 6700) {
                std::cout << "Se detectó como: Flat Tile con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size()
                          << " AR:" << ar << " Area: " << area << std::endl;
                return "Flat Tile";
            }else if (area > 11500 && area < 13000) {
                std::cout << "Se detectó como: P. 1x4 con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size()
                          << " AR:" << ar << " Area: " << area << std::endl;
                return "P. 1x4";
            }else if (area > 9000 && area < 10900) {
                std::cout << "Se detectó como: P. 1x2 Stick con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size()
                          << " AR:" << ar << " Area: " << area << std::endl;
                return "P. 1x2 Stick";
            }else if (area > 2700 && area < 3250){
                Rect r = boundingRect(approx);
                Point2f centros;
                float radio;
                minEnclosingCircle(approx, centros, radio);
                float ar = (float) r.width / (float) r.height;
                std::cout << "Se detectó como: H. 1x1 con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                          << " Radio: " << radio << " Area: " << area << std::endl;
                return "H. 1x1";
            }
            std::cout << "Se detectó como: Rect con atributos: Perimetro->" << perimeter << " Size: "
                      << approx.size()
                      << " AR:" << ar << " Area: " << area << std::endl;
            return "Rect";
        } else {
            Rect r = boundingRect(approx);
            Point2f centros;
            float radio;
            minEnclosingCircle(approx, centros, radio);
            float ar = (float) r.width / (float) r.height;
            if (perimeter > 950) {

                std::cout << "Se detectó como: MiniFigure - Atronaut con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                          << " Radio: " << radio << " Area: " << area << std::endl;
                return "MiniFigure - Atronaut";
            } else if (area > 2700 && area < 3250){
                std::cout << "Se detectó como: H. 1x1 con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                          << " Radio: " << radio << " Area: " << area << std::endl;
                return "H. 1x1";
            } else if (area > 7900 && area < 9000){
                std::cout << "Se detectó como: H. 2x1 con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                          << " Radio: " << radio << " Area: " << area << std::endl;
                return "H. 2x1";
            } else if (perimeter < 250){
                std::cout << "Se detectó como: Round 1x1 con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                          << " Radio: " << radio << " Area: " << area << std::endl;
                return "Round 1x1";
            } else if (area > 5000 && area < 6700){
                std::cout << "Se detectó como: Flat Tile con atributos: Perimetro->" << perimeter << " Size: "
                          << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                          << " Radio: " << radio << " Area: " << area << std::endl;
                return "Flat Tile";
            }
            std::cout << "Se detectó como: No Rect Holder con atributos: Perimetro->" << perimeter << " Size: "
                      << approx.size() << " AR:" << ar << std::endl << "Centro: " << centros.x << "," << centros.y
                      << " Radio: " << radio << " Area: " << area << std::endl;
            return "No Rect";
        }
    }

    return "garbage";
}

int main(int argc, char **argv) {
    // Get command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " image_file" << std::endl;
        return (-1);

    } // fi

    // Review given command line arguments
    std::cout << "-------------------------" << std::endl;
    for (int a = 0; a < argc; a++)
        std::cout << argv[a] << std::endl;
    std::cout << "-------------------------" << std::endl;

    // Read an image
    Mat image;
    image = imread(argv[1], 1);

    if (!image.data) {
        std::cerr << "Error: No image data" << std::endl;
        return (-1);

    } // fi

    //Save basename
    std::stringstream ss(argv[1]);
    std::string basename;
    getline(ss, basename, '.');

    //Creating the legoSet
    std::map<string, int> legoSet;
    legoSet.insert(std::pair<string, int>("Flat Tile", 3));
    legoSet.insert(std::pair<string, int>("H. 1x1", 1));
    legoSet.insert(std::pair<string, int>("MiniFigure - Atronaut", 1));
    legoSet.insert(std::pair<string, int>("Round 1x1", 2));
    legoSet.insert(std::pair<string, int>("H. 2x1", 1));
    legoSet.insert(std::pair<string, int>("P. 1x2 Stick", 1));
    legoSet.insert(std::pair<string, int>("P. 1x4", 1));
    legoSet.insert(std::pair<string, int>("P. 2x4", 3));
    std::map<string, int> fFigures;


    Mat resized;
    pyrDown(image, resized, Size(image.cols / 2, image.rows / 2));
    imwrite(basename + "_pyrDown.png", resized);
    float ratio = float(image.size().height / resized.size().height);
    Mat grayScale;
    cvtColor(image, grayScale, COLOR_BGR2GRAY);
    Mat bImage;
    GaussianBlur(grayScale, bImage, Size(5, 5), 0);
    imwrite(basename + "_blured.png", bImage);
    threshold(bImage, bImage, 45, 255, THRESH_BINARY);
    imwrite(basename + "_threshold.png", bImage);
    vector<vector<Point>> contours;
    cout << "Finding Cont\n";
    findContours(bImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    cout << "Success Finding Cont\n";
    Moments m;
    string response = "";
    float cx, cy;
    cout << "Gathering Moments :')\n";
    for (auto i = 0; i < contours.size(); i++) {
        m = moments(contours[i]);
        cx = float((m.m10 / m.m00) /** ratio*/);
        cy = float((m.m01 / m.m00) /** ratio*/);
        cout << "Detecting Shape\n";
        response = DetectShape(contours[i], cx, cy);
        if (response == "garbage")
            continue;
        cout << "Shape Detected: " + response << endl;

        vector<Point> pointCopy(contours[i]);
        /*for(auto j = 0; j < contours[i].size(); j++)
        {
        pointCopy[j].x *= ratio;
        pointCopy[j].y *= ratio;
     }*/
        cout << "Drawing Contours\n";
        drawContours(image, contours, i, Scalar(0, 255, 0), 2);
        cout << "Success Drawing Cont\n";

    }
    Mat redImage;
    extractChannel(resized, redImage, 2);
//Green
    Mat greenImage;
    extractChannel(resized, greenImage, 1);
//Blue
    Mat blueImage;
    extractChannel(resized, blueImage, 0);
    float sumaRed = 0;
    float sumaGreen = 0;
    float sumaBlue = 0;
    Point aux, actualPoint;
    for (auto i = 0; i < contours.size(); i++) {
        m = moments(contours[i]);
        cx = int((m.m10 / m.m00) /** ratio*/);
        cy = int((m.m01 / m.m00) /** ratio*/);
        response = DetectShape(contours[i], cx, cy);
        if (response == "garbage")
            continue;
        /*if (response == "Flat Tile"){
            sumaRed = 0;
            sumaGreen = 0;
            sumaBlue = 0;
            aux = Point(cx, cy);
            for (int i = 0; i < 8; i++) {
                actualPoint = aux + neighbors[i];
                std::cout<<sumaRed<<std::endl;
                sumaRed += (int)redImage.at<uchar>(actualPoint);
                sumaGreen += (int)greenImage.at<uchar>(actualPoint);
                sumaBlue += (int)blueImage.at<uchar>(actualPoint);
            }
            std::cout<<"Promedio R: "<<sumaRed/8<<std::endl;
            std::cout<<"Promedio G: "<<sumaGreen/8<<std::endl;
            std::cout<<"Promedio B: "<<sumaBlue/8<<std::endl;
            int promRed =(sumaRed/8);
            int promGreen =(sumaGreen/8);
            int promBlue =(sumaBlue/8);
            if (promRed>140 && promGreen > 140){
                if(promBlue>140){
                    response+= " A";
                } else {
                    response+= " B";
                }

            } else {
                response += " C";
            }
        }*/
        if (fFigures.count(response)>0){
            std::cout<<"Exíste! "<<response<<std::endl;
            fFigures[response]++;
            std::cout<<"Cuenta: "<<fFigures[response]<<std::endl;
        } else {
            std::cout<<"Creando! "<<response<<std::endl;
            fFigures.insert(std::pair<string, int> (response,1));
            std::cout<<"Cuenta: "<<fFigures[response]<<std::endl;
        }
        cout << "Drawing Text\n";
        putText(image, response, Point(cx - 20, cy), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 4);
        putText(image, response, Point(cx - 20, cy), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
        cout << "Success Drawing Text\n";

    }
    //Check if all pieces were found
    std::map<string, int>::iterator itEspected = legoSet.begin();
    bool notValid = false;
    while (itEspected != legoSet.end()){
        if (itEspected->second != fFigures[itEspected->first]){
            std::cout<<"No concuerdan. Esperado: "<<itEspected->second<<" en: "<<itEspected->first<<std::endl;
            std::cout<<"Recibido: "<<fFigures[itEspected->first]<<std::endl;
            notValid = true;
        }
        itEspected++;
    }
    if(notValid){
        std::cout<<"Fail"<<std::endl;
        putText(image,"Fail", Point(image.size().height/4,image.size().width/4),FONT_HERSHEY_SIMPLEX, 5, Scalar(0,0,0),10);
        putText(image,"Fail", Point(image.size().height/4,image.size().width/4),FONT_HERSHEY_SIMPLEX, 5, Scalar(43,164,232),7);
    } else {
        std::cout<<"Approved"<<std::endl;
        putText(image,"Approved", Point(image.size().height/4-35,image.size().width/4+20),FONT_HERSHEY_SIMPLEX, 4, Scalar(0,0,0),12);
        putText(image,"Approved", Point(image.size().height/4-35,image.size().width/4+20),FONT_HERSHEY_SIMPLEX, 4, Scalar(43,164,232),7);
    }

    //Write results
    imwrite(basename + "_response.png", image);


    return (0);
}

// eof - example.cxx
