// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QApplication>
#include <iostream>
#include <boost/filesystem.hpp>

#include "findmf/fileiopwiz/lcmsimagereader.h"
#include "findmf/application/lcmsimagefilter.h"
#include "findmf/application/featuresmapprinter.h"

#include "src/findmf/qtwidgets/TwoPanelImageWidget.h"
#include "findmf/fileio/helperfunctions.h"
#include "../apps/parseargExtract.h"
#include "findmf/algo/vigra/featurefinder.h"

int main(int argc, char *argv[])
{
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);
  ralab::findmf::apps::Params aparam;
  analysisParameters(aparam,vmgeneral);
  int res = 0;
  boost::filesystem::path p1,p2;

  int i = 0;
    {
      LOG(INFO)  << "i::::" << i << std::endl;
      p1 = createOutputs(aparam.infile,"");
      p2 = createOutputs(aparam.infile,"filtered");

      LOG(INFO)  << "ffname 2 :" << p1.string() << std::endl;
      LOG(INFO)  << "ffname 1 :" << p2.string() << std::endl;

      if( !boost::filesystem::exists(aparam.infile) )
        {
          LOG(ERROR)  << "could not find specified file :" << std::endl;
          LOG(ERROR)  << aparam.infile[i] << std::endl;
          return -1;
        }

      double ppm = aparam.ppm;
      ralab::LCMSImageReader sm(aparam.infile, ppm, aparam.rt2sum_ );
      ralab::LCMSImage mp;
      sm.getMap( 0 , aparam.minmass, aparam.maxmass, mp);
      LOG(INFO) << " " << mp.getMZsize() << " " << mp.getRTsize();

      ralab::LCMSImage mp2( mp );
      {
        ralab::LCMSImageFilter imgf;
        imgf.filterMap( mp2 , aparam.mzpixelwidth , aparam.rtpixelwidth, aparam.mzscale, aparam.rtscale);
      }
      ralab::FeatureFinder ff;
      ff.findFeature( mp2.getMap(), aparam.minintensity );
      ralab::FeaturesMap map;
      map.setMapDescription(mp2.getMapDescription());
      ff.extractFeatures(map,mp2.getMap());
      //this writes the accessor....
      ff.writeFeatures(aparam.outdir , p1.stem().string() );

      ralab::FeaturesMapPrinter fp;
      fp.writeFeatures(aparam.outdir , p1.stem().string(), map);

      ralab::MultiArrayVisSegments<int> mavL( ff.getLabels() );
      //write filtered data into mzML file
      sm.write( p2.string() , mp2 );
      if(1){
          QApplication app(argc, argv);
          ralab::TwoPaneledImageWidget tpi;
          ralab::MultiArrayVisLog<float> adapt1( mp2.getMap() );
          ralab::MultiArrayVisLog<float> adapt0( mp.getMap() );

          tpi.setTopMap(&adapt1);
          //tpi.setBottomMap(&mavL);
          tpi.setBottomMap(&adapt0);

          tpi.show();
          res = app.exec();
        }
    }
  return res;
}
