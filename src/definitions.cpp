#include <fstream>
#include <sstream>
#include <state-observation/tools/definitions.hpp>
#include <stdexcept>

namespace stateObservation
{
namespace tools
{
std::string matrixToString(const Matrix & mat)
{
  std::stringstream ss;
  ss << mat;
  return ss.str();
}

namespace detail
{

} // namespace detail

std::string vectorToString(const Vector & v)
{
  return matrixToString(v.transpose());
}

Matrix stringToMatrix(const std::string & str, Index rows, Index cols)
{
  Matrix m(Matrix::Zero(rows, cols));
  std::stringstream ss;
  ss << str;

  for(Index i = 0; i < rows; ++i)
  {
    for(Index j = 0; j < cols; ++j)
    {
      ss >> m(i, j);
    }
  }

  return m;
}

Vector stringToVector(const std::string & str, Index length)
{
  return stringToMatrix(str, length, 1);
}

Vector stringToVector(const std::string & str)
{
  Vector v;
  std::stringstream ss;
  ss << str;

  std::vector<double> doublecontainer;
  double component;
  bool readingVector = true;
  while(readingVector)
  {
    ss >> component;

    if(ss.fail())
    {
      readingVector = false;
    }
    else
    {
      doublecontainer.push_back(component);
    }
  }
  v.resize(Index(doublecontainer.size()));
  for(Index i = 0; i < Index(doublecontainer.size()); ++i)
  {
    v(i) = doublecontainer[size_t(i)];
  }

  return v;
}
} // namespace tools

} // namespace stateObservation
