#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp> // already trainded models for object detection
#include <iostream>
#include <string>    
    
using namespace cv;
using namespace std;
   
 /////////// face detection by haarcascade method /////////////////   
 int main() {   
    //importing images
    //string path = "Resources/lotFaces.jpg";
    //Mat img = imread(path); //MAT is the data tyoe intro by opencv

    VideoCapture cap(0);
    Mat img; ///= imread(path); //MAT is the data tyoe intro by opencv
    int noEyesFrame = 0;
    vector<double> ratioHist;
    const int graphWidth = 400;
    const int graphHeight = 100;    

    CascadeClassifier faceCascade, eyeCascade;
    faceCascade.load("Resources/haarcascade_frontalface_default.xml"); //loading the pre trainded model for face detection
    eyeCascade.load("Resources/haarcascade_eye_tree_eyeglasses.xml"); // loading the pre trainded model for eye detection


    if (faceCascade.empty()){cout<<"XML file not loaded for face detection"<<endl; return -1;}
    if (eyeCascade.empty()){cout<<"XML file not loaded for eye detection"<<endl; return -1;} // Haa cascade dont detect the frames in which eyes are closed so we will use if statement to determine open or closed eyes

    while(true){
        cap.read(img);
        vector<Rect> faces;
        vector<Rect> eyes;
        Mat graph = Mat::zeros(graphHeight, graphWidth, CV_8UC3); // for drawing brightness ratio graph

        faceCascade.detectMultiScale(img, faces, 1.1, 10); //scalefactor=1.1, minNeighbors=10
        eyeCascade.detectMultiScale(img, eyes, 1.1, 10); //scalefactor=1.1, minNeighbors=10

        for (int i=0; i<faces.size(); i++){
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255,0,255),2); //tl=top left br=bottom right
        }
        
        if (!eyes.empty()){
            noEyesFrame = 0; // resetting counter if eyes are detected
            for (int i=0; i<eyes.size(); i++){
                rectangle(img, eyes[i].tl(), eyes[i].br(), Scalar(255,255,0),2); //tl=top left br=bottom right

                Mat roiImg, grayEyes, thresh; // region of interest image for eyes 
                roiImg = img(eyes[i]);

                // Convert the eye region to grayscale to detect pupils or for further processing (open or closed eyes)
                cvtColor(roiImg, grayEyes, COLOR_BGR2GRAY);
                GaussianBlur(grayEyes, grayEyes, Size(7,7), 5); //blurring to reduce noise

                equalizeHist(grayEyes, grayEyes); //improve contrast

                threshold(grayEyes, thresh, 60, 255, THRESH_BINARY); //thresholding to get binary image

                imshow("Threshold Eye", thresh);// show thresholded eye image for deciding if to increase the threshold value

                double brigtRaio = (double)countNonZero(thresh) / (double)(thresh.total()); // calculating brightness ratio
                // Puting brightness ratio in history vector for showing it to the image
                ratioHist.push_back(brigtRaio);
                if (ratioHist.size() > graphWidth) {
                    ratioHist.erase(ratioHist.begin()); // keep the history size within graphWidth
                }

                string status = (brigtRaio > 0.7) ? "Open" : "Closed"; // simple condition to determine eye status

                // Draw waveform
                for (size_t j = 1; j < ratioHist.size(); j++) {
                    int x1 = static_cast<int>(j - 1);
                    int y1 = graphHeight - static_cast<int>(ratioHist[j - 1] * graphHeight);
                    int x2 = static_cast<int>(j);
                    int y2 = graphHeight - static_cast<int>(ratioHist[j] * graphHeight);

                    line(graph, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 3);
                }

                putText(img, "Eyes Open", Point(50,200), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0), 3);
            }
        }
        else{
            noEyesFrame++ ; // incrementing counter if eyes are detected
            cout<<endl<<"No eyes detected frame count: "<<noEyesFrame<<endl;

            if(noEyesFrame > 5){
                putText(img, "Eyes Closed", Point(50,200), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 3);
                cout<<endl<<"No eyes detected frame count in puttext eyes closed: "<<noEyesFrame<<endl;

                // Puting brightness ratio in history vector for showing it to the image
                ratioHist.push_back(0.05);
                if (ratioHist.size() > graphWidth) {
                    ratioHist.erase(ratioHist.begin()); // keep the history size within graphWidth
                }
                // Draw waveform
                for (size_t j = 1; j < ratioHist.size(); j++) {
                    int x1 = static_cast<int>(j - 1);
                    int y1 = graphHeight - static_cast<int>(ratioHist[j - 1] * graphHeight);
                    int x2 = static_cast<int>(j);
                    int y2 = graphHeight - static_cast<int>(ratioHist[j] * graphHeight);

                    line(graph, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 3);
                }

            }


        }

        // Overlay onto top-left of main frame
        graph.copyTo(img(Rect(10, 10, graph.cols, graph.rows)));


        imshow("Face and Eye detection", img);
        if (waitKey(25) >= 0) {
            break;// stop if any key is pressed
            }

     }
    return(0);
}
