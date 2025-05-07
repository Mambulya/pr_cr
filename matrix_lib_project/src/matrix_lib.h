#ifndef KMatrix_LIB_H
#define KMatrix_LIB_H

#include <cmath>
#include <iostream>
#include <utility>

class KMatrix {
 public:
  /*
  Правило пяти говорит о том, что в общем случае, если возникла необходимость
  самостоятельного определения одной из операций копирования, перемещения или
  разрушения объекта, то скорее всего для корректной работы нужно будет
  реализовать:
      - Деструктор
      - Конструктор копирования
      - Оператор присваивания копированием
      - Конструктор перемещения
      - Оператор присваивания перемещением
  */
  KMatrix();                      // дефолтный конструктор
  KMatrix(int rows, int cols);    // конструктор с параметрами
  ~KMatrix();                     // деструктор
  KMatrix(const KMatrix& other);  // конструктор копирования
  KMatrix(KMatrix&& other);       // конструктор перемещения

  /* Геттеры и Сеттеры для доступа к приватным полям класса */
  int GetColumns() const;
  int GetRows() const;
  void SetColumns(int cols);
  void SetRows(int rows);

  /* Основные функции */
  bool Equal(const KMatrix& other) const;
  void Sum(const KMatrix& other);
  void Sub(const KMatrix& other);
  void MultiplyNum(const double num);
  void MultiplyKMatrix(const KMatrix& other);
  KMatrix Transpose() const;
  KMatrix CalcComplements() const;  // алгебраические дополнения
  double Determinant() const;       // опрделитель матрицы
  KMatrix Inverse();

  /* перезагрузка операторов */
  KMatrix operator+(const KMatrix& other);
  KMatrix operator-(const KMatrix& other);
  KMatrix operator*(const KMatrix& other);
  KMatrix operator*(const double mul);
  bool operator==(const KMatrix& other) const;
  KMatrix& operator=(const KMatrix& other);
  KMatrix& operator=(KMatrix&& other);
  KMatrix operator+=(const KMatrix& other);
  KMatrix operator-=(const KMatrix& other);
  KMatrix operator*=(const KMatrix& other);
  KMatrix operator*=(const double mul);
  double& operator()(int row, int col);  // индексация по элементам
  double& operator()(int row, int col) const;

 private:
  /* Приватаные поля */
  int rows_, cols_;
  double** KMatrix_;

  /* Приватные вспомогательные методы */
  void CreateKMatrix();
  void Copy(const KMatrix& other);
  void Clear();
  void Fill(KMatrix& newKMatrix, int rows, int cols);
  void CheckEqual(const KMatrix& other) const;
  void CheckSquare() const;
  void FindComponent(KMatrix& complements) const;
  void FindMinor(KMatrix& minor, int row, int col) const;
  double FindDet() const;
  void CheckIndexExists(int row, int col) const;
};

#endif