// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <QApplication>
#include <iostream>
#include <boost/filesystem.hpp>

#include "findmf/fileiopwiz/lcmsimagereader.h"
#include "findmf/algo/vigra/lcmsimagefilter.h"
#include "findmf/application/featuresmapprinter.h"

#include "src/findmf/qtwidgets/TwoPanelImageWidget.h"
#include "findmf/fileio/helperfunctions.h"
#include "../apps/parseargExtract.h"
#include "findmf/algo/vigra/featurefinder.h"
#include "findmf/algo/vigra/ComputeFeatureStatistics.h"

int main(int argc, char *argv[])
{
  b_po::variables_map vmgeneral;
  parsecommandlineExtract(argc, argv, vmgeneral);
  ralab::findmf::apps::Params aparam;
  analysisParameters(aparam,vmgeneral);
  int res = 0;
  boost::filesystem::path p1,p2;

  {
    // LOG(INFO)  << "i::::" << i << std::endl;
    p1 = ralab::findmf::createOutputs(aparam.infile,"");
    p2 = ralab::findmf::createOutputs(aparam.infile,"filtered");

    if( !boost::filesystem::exists(aparam.infile) )
    {
      return -1;
    }

    ralab::findmf::datastruct::MSFileInfoPtr sip;
    try{
      ralab::findmf::SwathPropertiesReader swathPropReader(aparam.infile);
      sip = swathPropReader.getSwathInfo();
    } catch(std::exception & e){
      std::cerr << "infile : " << aparam.infile << std::endl;
      std::cerr  << "can't open file: " << e.what() << std::endl;
      return 0;
    }

    double ppm = aparam.ppm;
    pwiz::msdata::MSDataPtr msdataptr = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(aparam.infile));
    ralab::findmf::LCMSImageReader sm(msdataptr, sip, ppm, aparam.rt2sum_ );
    ralab::findmf::datastruct::LCMSImage mp;
    sm.getMap( 0 , aparam.minmass, aparam.maxmass, mp);
    // LOG(INFO) << " " << mp.getMZsize() << " " << mp.getRTsize();

    ralab::findmf::datastruct::LCMSImage mp2( mp );
    {
      ralab::findmf::LCMSImageFilterGauss imgf;
      // imgf.filterMap( mp2.getImageMap().getMap() , aparam.mzpixelwidth , aparam.rtpixelwidth, aparam.mzscale, aparam.rtscale);

      imgf.filter( mp2.getImageMap().getMap() , aparam.mzpixelwidth , aparam.rtpixelwidth, aparam.mzscale, aparam.rtscale);
      mp2.getImageMap().updateImageRange();
      std::cout << mp2.getImageMap().getImageMin() << std::endl;
      //imgf.filterMap();
    }

    // feature finding
    ralab::findmf::FeatureFinderLocalMax ff;
    ff.findFeature( mp2.getImageMap().getMap() , aparam.minintensity );

    ralab::findmf::datastruct::FeaturesMap map;
    map.setMapDescription(mp2.getMapDescription());
    ralab::findmf::ComputeFeatureStatistics cfs;
    cfs.extractFeatures(map,mp2.getImageMap().getMap(),ff.getLabels());

    //this writes the accessor....
   cfs.writeFeatures(aparam.outdir , p1.stem().string() );

    ralab::FeaturesMapPrinter fp;
    fp.writeFeatures(aparam.outdir , p1.stem().string(), map);

    ralab::MultiArrayVisSegments<int> mavL( ff.getLabels() );
    //write filtered data into mzML file

    //TODO check why this isn't working
    sm.write( p2.string() , mp2 );

    if(1){
      QApplication app(argc, argv);
      ralab::TwoPaneledImageWidget tpi;
      ralab::MultiArrayVisLog<float> adapt2( mp2.getImageMap().getMap() );

      ralab::MultiArrayVisAsinh<float> adapt1( mp2.getImageMap().getMap() );
      ralab::MultiArrayVisLog<float> adapt0( mp.getImageMap().getMap() );

      tpi.setTopMap(&mavL);
      //tpi.setBottomMap(&adapt0);
      tpi.setBottomMap(&adapt0);

      tpi.show();
      res = app.exec();
    }
  }
  return res;
}
