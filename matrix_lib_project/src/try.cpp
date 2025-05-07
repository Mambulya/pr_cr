
#include <iostream>
#include <opencv2/opencv.hpp>

#include "matrix_lib.h"

int main() {
  std::cout << "Starting...\n";
  KMatrix test = KMatrix(3, 4);
  std::cout << test.GetColumns();  // 4
  // Загружаем изображение
  cv::Mat image = cv::imread("image.jpg");
  if (image.empty()) {
    std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
    return -1;
  }

  // Отображаем изображение
  cv::imshow("Image", image);
  cv::waitKey(0);
  return 0;
}
