#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) const {
  int ans = 0;
  if (row > 0 && row < height_ - 1 && col > 0 && col < width_ - 1) {
    ans = GetEnergyLocation(row, col, row - 1, row + 1, col - 1, col + 1);
  }
  if (row == 0 && col > 0 && col < width_ - 1) {
    ans = GetEnergyLocation(row, col, height_ - 1, 1, col - 1, col + 1);
  }
  if (row == height_ - 1 && col > 0 && col < width_ - 1) {
    ans = GetEnergyLocation(row, col, 0, height_ - 2, col - 1, col + 1);
  }
  if (row > 0 && row < height_ - 1 && col == 0) {
    ans = GetEnergyLocation(row, col, row + 1, row - 1, 1, width_ - 1);
  }
  if (row > 0 && row < height_ - 1 && col == width_ - 1) {
    ans = GetEnergyLocation(row, col, row + 1, row - 1, width_ - 2, 0);
  }
  if (row == 0 && col == 0) {
    std::cout << 'x';
    ans =
        GetEnergyLocation(row, col, row + 1, height_ - 1, col + 1, width_ - 1);
  }
  if (row == height_ - 1 && col == 0) {
    ans = GetEnergyLocation(row, col, 0, row - 1, col + 1, width_ - 1);
  }
  if (row == 0 && col == width_ - 1) {
    ans = GetEnergyLocation(row, col, row + 1, height_ - 1, col - 1, 0);
  }
  if (row == height_ - 1 && col == width_ - 1) {
    ans = GetEnergyLocation(row, col, 0, row - 1, col - 1, 0);
  }
  return ans;
}

int SeamCarver::GetEnergyLocation(
    int row, int col, int row_i, int row_j, int col_i, int col_j) const {
  int col_energy = 0;
  int row_energy = 0;
  int r = 0;
  int g = 0;
  int b = 0;
  r = image_.GetPixel(row_i, col).GetRed() -
      image_.GetPixel(row_j, col).GetRed();
  g = image_.GetPixel(row_i, col).GetGreen() -
      image_.GetPixel(row_j, col).GetGreen();
  b = image_.GetPixel(row_i, col).GetBlue() -
      image_.GetPixel(row_j, col).GetBlue();
  col_energy = r * r + g * g + b * b;
  std::cout << r;
  r = image_.GetPixel(row, col_i).GetRed() -
      image_.GetPixel(row, col_j).GetRed();
  g = image_.GetPixel(row, col_i).GetGreen() -
      image_.GetPixel(row, col_j).GetGreen();
  b = image_.GetPixel(row, col_i).GetBlue() -
      image_.GetPixel(row, col_j).GetBlue();
  row_energy = r * r + g * g + b * b;
  return row_energy + col_energy;
}

int* SeamCarver::GetVerticalSeam() const {
  int* path = new int[height_];
  int** map = SetVerticalSumEnergyMap();
  int** energy = SetEnergyMap();
  int count = 0;
  int search = 0;
  int col = 0;
  for (int col = 0; col < width_; col++) {
    if (map[0][count] > map[0][col]) {
      count = col;
    }
  }
  path[0] = count;
  for (int row = 0; row < height_ - 1; row++) {
    col = path[row];
    search = map[row][col] - energy[row][col];
    if (col > 0 && col < width_ - 1) {
      if (map[row + 1][col] == search) {
        path[row + 1] = col;
      } else if (map[row + 1][col - 1] == search) {
        path[row + 1] = col - 1;
      } else {
        path[row + 1] = col + 1;
      }
    } else if (col == 0) {
      if (map[row + 1][col] == search) {
        path[row + 1] = col;
      } else {
        path[row + 1] = col + 1;
      }
    } else {
      if (map[row + 1][col] == search) {
        path[row + 1] = col;
      } else {
        path[row + 1] = col - 1;
      }
    }
  }
  ClearMap(map, energy);
  return path;
}

void SeamCarver::ClearMap(int** map) const {
  for (int i = 0; i < height_; i++) {
    delete[] map[i];
  }
  delete[] map;
  map = nullptr;
}

void SeamCarver::ClearMap(int** map1, int** map2) const {
  ClearMap(map1);
  ClearMap(map2);
}

int** SeamCarver::SetVerticalSumEnergyMap() const {
  int** map = SetEnergyMap();
  for (int row = height_ - 2; row > -1; row--) {
    for (int col = 0; col < width_; col++) {
      SetVerticalSingleEnergyMap(row, col, map);
    }
  }
  return map;
}

void SeamCarver::SetVerticalSingleEnergyMap(int row, int col, int** map) const {
  int mid = 0;
  int left = 0;
  int right = 0;
  if (col > 0 && col < width_ - 1) {
    mid = map[row + 1][col];
    right = map[row + 1][col + 1];
    left = map[row + 1][col - 1];
    map[row][col] += Min(mid, left, right);
  }
  if (col == 0) {
    mid = map[row + 1][col];
    right = map[row + 1][col + 1];
    map[row][col] += Min(mid, right);
  }
  if (col == width_ - 1) {
    mid = map[row + 1][col];
    left = map[row + 1][col - 1];
    map[row][col] += Min(mid, left);
  }
}

int Min(int a, int b, int c) {
  if (a <= b && a <= c) {
    return a;
  }
  if (b <= c) {
    return b;
  }
  return c;
}

int Min(int a, int b) {
  if (a <= b) {
    return a;
  }
  return b;
}

int** SeamCarver::SetEnergyMap() const {
  int** map = new int*[height_];
  for (int i = 0; i < height_; i++) {
    map[i] = new int[width_];
  }
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      map[i][j] = GetEnergy(i, j);
    }
  }
  return map;
}

void SeamCarver::RemoveVerticalSeam() {
  int* path = GetVerticalSeam();
  image_.RemoveVerticalSeam(path);
  width_--;
  delete[] path;
  path = nullptr;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* path = GetHorizontalSeam();
  image_.RemoveHorizontalSeam(path);
  height_--;
  delete[] path;
  path = nullptr;
}

int* SeamCarver::GetHorizontalSeam() const {
  int* path = new int[width_];
  int** map = SetHorizontalSumEnergyMap();
  int** energy = SetEnergyMap();
  int count = 0;
  int search = 0;
  int row = 0;
  for (int row = 0; row < height_; row++) {
    if (map[count][0] > map[row][0]) {
      count = row;
    }
  }
  path[0] = count;
  for (int col = 0; col < width_ - 1; col++) {
    row = path[col];
    search = map[row][col] - energy[row][col];
    if (row > 0 && row < height_ - 1) {
      path[col + 1] = path[col] + Select(map[row - 1][col + 1],
                                         map[row][col + 1],
                                         map[row + 1][col + 1],
                                         search);
    } else if (row == 0) {
      path[col + 1] =
          Select(-1, map[row][col + 1], map[row + 1][col + 1], search);
    } else {
      path[col + 1] =
          height_ +
          Select(map[row - 1][col + 1], map[row][col + 1], -1, search) - 1;
    }
  }
  ClearMap(map, energy);
  return path;
}

void SeamCarver::SetHorizontalSingleEnergyMap(int row,
                                              int col,
                                              int** map) const {
  int mid = 0;
  int up = 0;
  int down = 0;
  if (row > 0 && row < height_ - 1) {
    mid = map[row][col + 1];
    up = map[row - 1][col + 1];
    down = map[row + 1][col + 1];
    map[row][col] += Min(mid, up, down);
  }
  if (row == 0) {
    mid = map[row][col + 1];
    down = map[row + 1][col + 1];
    map[row][col] += Min(mid, down);
  }
  if (row == height_ - 1) {
    mid = map[row][col + 1];
    up = map[row - 1][col + 1];
    map[row][col] += Min(mid, up);
  }
}

int** SeamCarver::SetHorizontalSumEnergyMap() const {
  int** map = SetEnergyMap();
  for (int col = width_ - 2; col > -1; col--) {
    for (int row = 0; row < height_; row++) {
      SetHorizontalSingleEnergyMap(row, col, map);
    }
  }
  return map;
}

int Select(int a, int b, int c, int search) {
  if (b == search) {
    return 0;
  }
  if (a == search) {
    return -1;
  }
  if (c == search) {
    return 1;
  }
  throw "error";
}
