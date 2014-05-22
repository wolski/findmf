CREATE TABLE features ( id integer primary key, idmap int, idswath int,centerOfMassMZ real, centerOfMassRT real, MZ real, RT real ,MZSD real, RTSD real, MZSKEW real, RTSKEW real, MZKURT real, RTKURT real, Max real, Count integer, Volume real, maximumLocationMZ real, maximumLocationRT real, minMZIndex real, mzExtend real, minRTIndex real, rtExtend real, mzProjection blob , rtProjection blob , FOREIGN KEY(id) REFERENCES feature_index(id) , FOREIGN KEY(idswath) REFERENCES mapinfo(id));
CREATE TABLE feature_index(id integer primary key, minMZsw real, maxMZsw real,minRT real, maxRT real, minMZ real,maxMZ real);
CREATE TABLE mapinfo ( id integer primary key, mslevel int, minMZsw real,maxMZsw real,minMZ real ,maxMZ real ,minRT real ,maxRT real ,rts blob,mzs blob);