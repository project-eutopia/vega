// clang++ --std=c++11 -lvega -lz ct_files_to_phantom.cpp -o ct_files_to_phantom

#include <stdint.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

#include <vega/dicom/file.h>
#include <vega/dictionary/dictionary.h>

template <typename T>
class Matrix {
  private:
    unsigned m_rows;
    unsigned m_cols;
    std::vector<T> m_data;

  public:
    Matrix(unsigned rows, unsigned cols)
      :
        m_rows(rows),
        m_cols(cols),
        m_data(rows*cols, 0)
    {}

    unsigned rows() const { return m_rows; }
    unsigned cols() const { return m_cols; }

    T& operator()(unsigned row, unsigned col) {
      return m_data[col + m_cols*row];
    }
};

// HU = 1000 * (u - uwater) / (uwater - uair)
// HU * (uwater - uair) / 1000 + uwater = u
double density_from_hu(double hu) {
  return hu * (1.0 - 0.00125) / 1000 + 1.0;
}

int main(int argc, char *argv[]) {
  vega::dictionary::set_dictionary(std::getenv("VEGA_DICTIONARY"));

  std::vector<double> thicknesses;
  std::vector<std::tuple<double,double,double>> bottom_lefts;
  std::vector<std::tuple<double,double,double>> top_rights;
  std::vector<Matrix<double>> densities;

  std::vector<std::tuple<unsigned,double>> index_and_z_positions;

  for (unsigned i = 1; i < argc; ++i) {
    vega::dicom::File file{std::string(argv[i])};

    thicknesses.push_back(file.data_set()->element<vega::dictionary::SliceThickness>()->manipulator()->at(0));

    unsigned rows = file.data_set()->element<vega::dictionary::Rows>()->manipulator()->at(0);
    unsigned cols = file.data_set()->element<vega::dictionary::Columns>()->manipulator()->at(0);
    densities.emplace_back(rows, cols);

    double delta_x = file.data_set()->element<vega::dictionary::PixelSpacing>()->manipulator()->at(0);
    double delta_y = file.data_set()->element<vega::dictionary::PixelSpacing>()->manipulator()->at(1);

    auto image_position = file.data_set()->element<vega::dictionary::ImagePositionPatient>()->manipulator();
    bottom_lefts.push_back(std::make_tuple(double(image_position->at(0)) - delta_x/2.0, double(image_position->at(1)) - delta_y/2.0, double(image_position->at(2)) - thicknesses.back()/2.0));
    top_rights.push_back(std::make_tuple(double(image_position->at(0)) + delta_x*(cols-0.5), double(image_position->at(1)) + delta_y*(rows-0.5), double(image_position->at(2)) + thicknesses.back()/2.0));

    index_and_z_positions.push_back(std::make_tuple(i-1, double(image_position->at(2))));

    unsigned bits = file.data_set()->element<vega::dictionary::BitsAllocated>()->manipulator()->at(0);
    double slope = file.data_set()->element<vega::dictionary::RescaleSlope>()->manipulator()->at(0);
    double intercept = file.data_set()->element<vega::dictionary::RescaleIntercept>()->manipulator()->at(0);
    bool pixels_are_signed = file.data_set()->element<vega::dictionary::PixelRepresentation>()->manipulator()->at(0) == 1;

    if (bits == 8) {
      auto manipulator = file.data_set()->element<vega::dictionary::PixelData_OB>()->manipulator();
      for (unsigned row = 0; row < rows; ++row) {
        for (unsigned col = 0; col < cols; ++col) {
          densities.back()(row, col) = density_from_hu(intercept + slope * (pixels_are_signed ? manipulator->at(col + cols*row).i : manipulator->at(col + cols*row).u));
        }
      }
    }
    else if (bits == 16) {
      auto manipulator = file.data_set()->element<vega::dictionary::PixelData_OW>()->manipulator();
      for (unsigned row = 0; row < rows; ++row) {
        for (unsigned col = 0; col < cols; ++col) {
          densities.back()(row, col) = density_from_hu(intercept + slope * (pixels_are_signed ? manipulator->at(col + cols*row).i : manipulator->at(col + cols*row).u));
        }
      }
    }
    else {
      throw std::runtime_error("Invalid bits");
    }
  }

  // Sort by z
  std::sort(index_and_z_positions.begin(), index_and_z_positions.end(), [](const std::tuple<unsigned,double>& a, const std::tuple<unsigned,double>& b) {
    return std::get<1>(a) < std::get<1>(b);
  });

  unsigned first_index = std::get<0>(index_and_z_positions.front());
  unsigned last_index  = std::get<0>(index_and_z_positions.back());

  unsigned nx = densities.front().cols();
  unsigned ny = densities.front().rows();
  unsigned nz = index_and_z_positions.size();

  {
    std::ofstream voxel_grid;
    voxel_grid.open("voxel_grid.json", std::ofstream::out);

    voxel_grid << "{\"voxel_grid\": {\"nx\": " << nx << ", \"ny\": " << ny << ", \"nz\": " << nz << ", ";
    voxel_grid << "\"bottom_left\": [" << std::get<0>(bottom_lefts[first_index])/10.0 << ", " << std::get<1>(bottom_lefts[first_index])/10.0 << ", " << std::get<2>(bottom_lefts[first_index])/10.0 << "], ";
    voxel_grid << "\"top_right\": [" << std::get<0>(top_rights[last_index])/10.0 << ", " << std::get<1>(top_rights[last_index])/10.0 << ", " << std::get<2>(top_rights[last_index])/10.0 << "]}}" << std::endl;

    voxel_grid.close();
  }

  {
    int32_t i;
    float density;

    std::ofstream phantom;
    phantom.open("densities.phantom", std::ofstream::out | std::ofstream::binary);

    i = nx;
    phantom.write(reinterpret_cast<const char*>(&i), sizeof(i));
    i = ny;
    phantom.write(reinterpret_cast<const char*>(&i), sizeof(i));
    i = nz;
    phantom.write(reinterpret_cast<const char*>(&i), sizeof(i));

    for (unsigned z = 0; z < nz; ++z) {
      for (unsigned y = 0; y < ny; ++y) {
        for (unsigned x = 0; x < nx; ++x) {
          density = densities[std::get<0>(index_and_z_positions[z])](x, y);
          phantom.write(reinterpret_cast<const char*>(&density), sizeof(density));
        }
      }
    }

    phantom.close();
  }

  return 0;
}
