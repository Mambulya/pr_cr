#include "matrix_lib.h"

#include <utility>

// Дефолтный конструктор
// Списки инициализации представляют перечисления инициализаторов
// для каждой из переменных и констант через двоеточие после
// списка параметров конструктора:
KMatrix::KMatrix() : rows_{}, cols_{}, KMatrix_{} {}

// конструктор с параметрами
KMatrix::KMatrix(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw std::invalid_argument(
        "Количество столбцов и строк в матрице не может быть мнеьше 1");
  }
  rows_ = rows;
  cols_ = cols;
  CreateKMatrix();
}

// приватный метод: выделение памяти для матрицы и заполнения нулями
void KMatrix::CreateKMatrix() {
  KMatrix_ = new double *[rows_]();
  for (int i = 0; i < rows_; i++) {
    KMatrix_[i] = new double[cols_]();
  }
}

// Деструктор
KMatrix::~KMatrix() { Clear(); }

// Конструктор копирования
KMatrix::KMatrix(const KMatrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  if (&other == this) {
    throw std::logic_error("нельзя копировать в себя");
  }
  Copy(other);
}

/**
 * @brief копирование матрицы
 * @param other объект-матрица которую хотим копировать
 */
void KMatrix::Copy(const KMatrix &other) {
  CreateKMatrix();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      KMatrix_[i][j] = other.KMatrix_[i][j];
    }
  }
}

// Конструктор перемещения
KMatrix::KMatrix(KMatrix &&other) {
  KMatrix_ = other.KMatrix_;
  rows_ = other.rows_;
  cols_ = other.cols_;

  other.KMatrix_ = nullptr;
  other.cols_ = 0;
  other.rows_ = 0;
}

// Приватный метод:
// очистка памяти и установка количества строк и столбцов в нули
void KMatrix::Clear() {
  if (KMatrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] KMatrix_[i];
    }
    delete[] KMatrix_;
  }
  KMatrix_ = nullptr;
  rows_ = 0;
  cols_ = 0;
}

// Геттер - получить количество строк
int KMatrix::GetRows() const { return rows_; }

// Геттер - получить количество столбцов
int KMatrix::GetColumns() const { return cols_; }

// Приватный метод для копирования значений матрицы
void KMatrix::Fill(KMatrix &newKMatrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      newKMatrix.KMatrix_[i][j] = KMatrix_[i][j];
    }
  }
}

/**
 * Сеттер для установления количества строк, при уменьшении
 * размера матрицы лишнее отрасыватеся
 */
void KMatrix::SetRows(int rows) {
  KMatrix newKMatrix(rows, cols_);
  double edge = rows_;
  if (rows < rows_) edge = rows;
  Fill(newKMatrix, edge, cols_);
  *this = newKMatrix;
}

/**
 * Сеттер для установления количество столбцов, при уменьшении
 * размера матрицы лишнее отрасыватеся
 */
void KMatrix::SetColumns(int cols) {
  KMatrix newKMatrix(rows_, cols);
  double edge = cols_;
  if (cols < cols_) edge = cols;
  Fill(newKMatrix, rows_, edge);
  *this = newKMatrix;
}

// проверяет матрицы на равенство
bool KMatrix::Equal(const KMatrix &other) const {
  bool flag = true;
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    flag = false;
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (fabs(KMatrix_[i][j] - other.KMatrix_[i][j]) >= 1.0e-07) {
          flag = false;
        }
      }
    }
  }
  return flag;
}

// сложение матриц, перед этим выполняется проверка на
// пазмерность двух матриц, если размеры не равны,
// то кидается исключение
void KMatrix::Sum(const KMatrix &other) {
  CheckEqual(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      KMatrix_[i][j] += other.KMatrix_[i][j];
    }
  }
}

// вычитание матриц, перед этим выполняется проверка на
// размерность двух матриц, если размеры не равны,
// то кидается исключение
void KMatrix::Sub(const KMatrix &other) {
  CheckEqual(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      KMatrix_[i][j] -= other.KMatrix_[i][j];
    }
  }
}

// проверка матриц на одинаковый размер
void KMatrix::CheckEqual(const KMatrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("количество строк и столбцов не равны");
  }
}

// умножение матрицы на число
void KMatrix::MultiplyNum(const double num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      KMatrix_[i][j] *= num;
    }
  }
}

// Умножение двух матриц, с предварительной проверкой
// чтобы количество строк одной матрицы было равно количеству столбцов
// другой матрицы
void KMatrix::MultiplyKMatrix(const KMatrix &other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument("Invalid sizes of matrices for MultiplyNuming");
  }
  KMatrix res(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      res(i, j) = 0;
      for (int k = 0; k < cols_; k++) {
        res(i, j) += KMatrix_[i][k] * other.KMatrix_[k][j];
      }
    }
  }
  *this = res;
}

// транспонирование матрицы
KMatrix KMatrix::Transpose() const {
  KMatrix transposed(cols_, rows_);
  for (int i = 0; i < transposed.rows_; i++) {
    for (int j = 0; j < transposed.cols_; j++) {
      transposed.KMatrix_[i][j] = KMatrix_[j][i];
    }
  }
  return transposed;
}

// Вычисляет матрицу алгебраических дополнений
// текущей матрицы и возвращает ее
KMatrix KMatrix::CalcComplements() const {
  CheckSquare();  // если матрица не квадратная - кидает исключение
  if (rows_ == 1) {
    throw std::logic_error("Размерность должна быть больше 1");
  }
  KMatrix result(rows_, cols_);
  FindComponent(result);
  return result;
}

// Проверка на квадратность матрицы
void KMatrix::CheckSquare() const {
  if (rows_ != cols_) {
    throw std::logic_error("Матрица не квадратная");
  }
}

// вспомогательная функция для нахождения матрицы кофакторов
void KMatrix::FindComponent(KMatrix &complements) const {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      KMatrix minor(rows_ - 1, cols_ - 1);
      FindMinor(minor, i, j);
      complements.KMatrix_[i][j] = pow(-1, (i + j)) * minor.Determinant();
      minor.Clear();
    }
  }
}

// Нахождение минора матрицы
void KMatrix::FindMinor(KMatrix &minor, int row, int col) const {
  int RowCounter = 0, ColCounter = 0;
  for (int i = 0; i < rows_; i++) {
    if (i != row) {
      ColCounter = 0;
      for (int j = 0; j < cols_; j++) {
        if (j != col) {
          minor.KMatrix_[RowCounter][ColCounter] = KMatrix_[i][j];
          ColCounter++;
        }
      }
      RowCounter++;
    }
  }
}

// Вычисляет и возвращает определитель текущей матрицы
double KMatrix::Determinant() const {
  CheckSquare();
  return FindDet();
}

// вспомогательный метод для нахождениея дтерминанта
double KMatrix::FindDet() const {
  double total = 0;
  if (rows_ == 1) {
    total = KMatrix_[0][0];
  } else {
    for (int j = 0; j < cols_; j++) {
      KMatrix minor(rows_ - 1, cols_ - 1);
      FindMinor(minor, 0, j);
      total += KMatrix_[0][j] * pow(-1, j) * minor.FindDet();
      minor.Clear();
    }
  }
  return total;
}

// Вычисляет и возвращает обратную матрицу
KMatrix KMatrix::Inverse() {
  const double determinant = Determinant();
  if (fabs(determinant) <= 1.0e-7) {
    throw std::logic_error("Определитель не может быть 0");
  }
  KMatrix inversed = CalcComplements().Transpose();
  inversed *= (1.0 / determinant);
  return inversed;
}

/*---------ПЕРЕЗАГРУЗКА ОПЕРАТОРОВ----------*/

KMatrix KMatrix::operator+(const KMatrix &other) {
  KMatrix result(*this);
  result.Sum(other);
  return result;
}

KMatrix KMatrix::operator-(const KMatrix &other) {
  KMatrix result(*this);
  result.Sub(other);
  return result;
}

KMatrix KMatrix::operator*(const KMatrix &other) {
  KMatrix result(*this);
  result.MultiplyKMatrix(other);
  return result;
}

KMatrix KMatrix::operator*(const double mul) {
  KMatrix result(*this);
  result.MultiplyNum(mul);
  return result;
}

bool KMatrix::operator==(const KMatrix &other) const { return Equal(other); }

KMatrix &KMatrix::operator=(const KMatrix &other) {
  if (this == &other) {
    return *this;
  }

  Clear();
  rows_ = other.rows_;
  cols_ = other.cols_;
  Copy(other);
  return *this;
}

KMatrix &KMatrix::operator=(KMatrix &&other) {
  if (this == &other) {
    return *this;
  }
  Clear();
  rows_ = other.rows_;
  cols_ = other.cols_;
  KMatrix_ = other.KMatrix_;

  other.rows_ = 0;
  other.cols_ = 0;
  other.KMatrix_ = nullptr;
  return *this;
}

KMatrix KMatrix::operator+=(const KMatrix &other) {
  Sum(other);
  return *this;
}

KMatrix KMatrix::operator-=(const KMatrix &other) {
  Sub(other);
  return *this;
}

KMatrix KMatrix::operator*=(const KMatrix &other) {
  MultiplyKMatrix(other);
  return *this;
}

KMatrix KMatrix::operator*=(const double mul) {
  MultiplyNum(mul);
  return *this;
}

double &KMatrix::operator()(int row, int col) {
  CheckIndexExists(row, col);
  return KMatrix_[row][col];
}

double &KMatrix::operator()(int row, int col) const {
  CheckIndexExists(row, col);
  return KMatrix_[row][col];
}

// вспомогательная функция для проверки, что индексы не за пределами матрицы
void KMatrix::CheckIndexExists(int row, int col) const {
  if (row < 0) {
    throw std::out_of_range("Кол-во строк должно быть больше 0");
  } else if (col < 0) {
    throw std::out_of_range("Кол-во столбцов должно быть больше 0");
  } else if (row >= rows_) {
    throw std::out_of_range("Кол-во строк за пределами матрицы");
  } else if (col >= cols_) {
    throw std::out_of_range("Кол-во столбцов за пределами матрицы");
  }
}