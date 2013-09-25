// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


#ifndef SQLFINDMFSTORAGE_H
#define SQLFINDMFSTORAGE_H

#include <string>
#include <boost/filesystem.hpp>
#include <glog/logging.h>
#include "soci/soci.h"
#include "soci/soci-backend.h"
#include "soci/soci-sqlite3.h"

namespace ralab{

  struct SQLFindMFStorage{

    soci::session db_;
    std::string dblocation_;
soci::statement st_;
    //open resources
    SQLFindMFStorage(const std::string & dblocation):dblocation_(dblocation),
      db_(soci::sqlite3,dblocation)
    {
      st_.get_affected_rows()
    }



    //open resources
    SQLFindMFStorage(const std::string & output_directory,
                     const std::string & outfileprefix):
      dblocation_(makelocation(output_directory,outfileprefix)),
      db_(soci::sqlite3,dblocation_)
    {
    }

    SQLFindMFStorage():db_(),dblocation_(){}



    //open resource (alternative constructor)
    static std::string makelocation(const std::string & output_directory,
                                    const std::string & outfileprefix)
    {
      boost::filesystem::path p(output_directory);
      std::string outfile1 = outfileprefix;
      outfile1 +=".sqlite";
      boost::filesystem::path p1 = p;
      p1 /= outfile1;
      std::string dblocation = p1.string();
      return dblocation;
    }

  private:

    SQLFindMFStorage(const SQLFindMFStorage & rh){}


  public:

    //close resources RAII
    ~SQLFindMFStorage(){
      LOG(INFO) << "db closing !" ;
      db_.close();
    }



    bool isOpen(){
      return true;//db_.open();
    }

    void openDB(std::string & location){
      db_.open(soci::sqlite3,location);
    }

    soci::session & session(){
      return db_;
    }

    std::string & getDBLocation(){
      return dblocation_;
    }

    // close db
    void closeDB(){
      db_.close();
    }

    //opens transaction
    void transaction(){
      //db_.transaction();
    }


    void commit(){
      LOG(INFO) << "db committed";
      //db_.commit();
    }

  };
}


#endif // SQLFEATURESTORAGE_H
