// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "base/ms/deisotoper.h"
#include <gtest/gtest.h>


namespace {

  /*!\brief Helper function to generate spectra with Gaussian peaks.
         \ingroup FILTER
         */

  // The fixture for testing class Foo.
  class DeisotoperTest : public ::testing::Test {
  protected:
    std::vector<double> mass1v , mass2v , intensity1v, intensity2v;
    std::vector<double> mass3v, intensity3v;
    std::vector<double> mass4v, intensity4v;

    // You can remove any or all of the following functions if its body
    // is empty.
    DeisotoperTest() {
      // You can do set-up work for each test here.
    }
    virtual ~DeisotoperTest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {


      // Code here will be called immediately after the constructor (right
      // before each test).

      //theoretical patterns
      double mass1[] = {300.17921,  301.18207, 302.18434, 303.18674, 304.18907, 305.19145, 306.19441 };
      double intens1[] = {84.14, 13.91, 1.77, 0.17,0.01,0,0};
      intensity1v.assign(intens1,intens1 + sizeof(intens1)/sizeof(double));
      mass1v.assign(mass1,mass1+sizeof(mass1)/sizeof(double));


      double mass2[] = {750.50837, 751.5113, 752.51391, 753.51644, 754.51892,755.52138};
      double intens2[] = {64.75,26.96,6.8,1.27,0.19,0.02};
      intensity2v.assign(intens2,intens2 + sizeof(intens2)/sizeof(double));
      mass2v.assign(mass2,mass2+sizeof(mass2)/sizeof(double));


      //theoretical pattern // connected at z=1 but not at higher charges
      double mass4[] = {100 , 101.5 , 102 , 103 , 103.5 , 104 , 104.5 };
      double intens4[] = {84.14, 13.91, 1.77, 84.14, 13.91, 1.77,  0.17};

      intensity4v.assign(intens4,intens4 + sizeof(intens4)/sizeof(double));
      mass4v.assign(mass4,mass4+sizeof(mass4)/sizeof(double));

      //experimental pattern
      double mass3[] = {726.068, 726.337, 726.589, 726.842, 727.343, 727.846, 728.346,
              728.846, 729.348, 730.248, 730.336, 730.581, 730.836};

      double intens3[] = {6.77850e+03, 2.81688e+04, 6.66884e+04, 1.22032e+07,
              9.90405e+06, 4.61409e+06, 1.50973e+06, 3.33996e+05, 5.09421e+04,
              1.15869e+03, 2.14788e+03, 5.37853e+03, 5.79094e+02};
      intensity3v.assign(intens3,intens3 + sizeof(intens3)/sizeof(double));
      mass3v.assign(mass3,mass3+sizeof(mass3)/sizeof(double));

    }

    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }



    // Objects declared here can be used by all tests in the test case for Foo.
  };

  // Tests that the Foo::Bar() method does Abc.
  TEST_F(DeisotoperTest, testdeisotop) {
    std::vector<int> z;
    z.push_back(1);
    z.push_back(2);
    z.push_back(3);
    z.push_back(4);

    ralab::base::ms::Deisotoper x(z.begin(),z.end(),5.);
    //x.deisotop(mass3v.begin() , mass3v.end(), intensity3v.begin());

  }

}//end namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

