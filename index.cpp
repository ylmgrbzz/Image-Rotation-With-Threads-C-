#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

// own function to rotate image 90 degrees
Mat rotateImage(const Mat& image)
{
    int rows = image.rows;
    int cols = image.cols;

    // Yeni döndürülmüş görüntüyü saklamak için bir Mat oluştur
    Mat rotatedImage(cols, rows, image.type());

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotatedImage.at<Vec3b>(j, rows - 1 - i) = image.at<Vec3b>(i, j);
        }
    }

    return rotatedImage;
}

int main()
{
    // Input photo path
    string photo_path = "../a.jpg";

    // Read photo
    Mat photo = imread(photo_path);

    // Check if photo is empty
    if (photo.empty())
    {
        cout << "photo is empty" << endl;
        return -1;
    }

    // Divide photo into 4 parts
    int rows = photo.rows;
    int cols = photo.cols;
    int part_rows = rows / 2;
    int part_cols = cols / 2;

    Mat part1 = photo(Rect(0, 0, part_cols, part_rows));
    Mat part2 = photo(Rect(part_cols, 0, part_cols, part_rows));
    Mat part3 = photo(Rect(0, part_rows, part_cols, part_rows));
    Mat part4 = photo(Rect(part_cols, part_rows, part_cols, part_rows));

    // Rotate parts of photo using threads
    Mat rotated_part1;
    Mat rotated_part2;
    Mat rotated_part3;
    Mat rotated_part4;

    auto start1 = chrono::high_resolution_clock::now();
    thread thread1([&]() {
        rotated_part1 = rotateImage(part1);
    });
    auto end1 = chrono::high_resolution_clock::now();

    auto start2 = chrono::high_resolution_clock::now();
    thread thread2([&]() {
        rotated_part2 = rotateImage(part2);
    });
    auto end2 = chrono::high_resolution_clock::now();

    auto start3 = chrono::high_resolution_clock::now();
    thread thread3([&]() {
        rotated_part3 = rotateImage(part3);
    });
    auto end3 = chrono::high_resolution_clock::now();

    auto start4 = chrono::high_resolution_clock::now();
    thread thread4([&]() {
        rotated_part4 = rotateImage(part4);
    });
    auto end4 = chrono::high_resolution_clock::now();

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    Mat new_photo = Mat::zeros(cols, rows, photo.type());

    // copy rotated parts of photo to new photo
    rotated_part1.copyTo(new_photo(Rect(part_rows, 0, part_rows, part_cols)));
    rotated_part2.copyTo(new_photo(Rect(part_rows, part_cols, part_rows, part_cols)));
    rotated_part3.copyTo(new_photo(Rect(0, 0, part_rows, part_cols)));
    rotated_part4.copyTo(new_photo(Rect(0, part_cols, part_rows, part_cols)));

    imwrite("new_photo.jpg", new_photo);

    // Calculate and print the execution times
    chrono::duration<double> elapsed1 = end1 - start1;
    chrono::duration<double> elapsed2 = end2 - start2;
    chrono::duration<double> elapsed3 = end3 - start3;
    chrono::duration<double> elapsed4 = end4 - start4;

    cout << fixed << setprecision(6);  // İki basamaklı ondalık gösterim için

    cout << "Thread 1 Execution Time: " << elapsed1.count() << " seconds" << endl;
    cout << "Thread 2 Execution Time: " << elapsed2.count() << " seconds" << endl;
    cout << "Thread 3 Execution Time: " << elapsed3.count() << " seconds" << endl;
    cout << "Thread 4 Execution Time: " << elapsed4.count() << " seconds" << endl;

    return 0;
}