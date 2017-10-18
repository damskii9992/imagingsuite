#include <vector>
#include <string>

#include <QString>
#include <QtTest>

#include <base/timage.h>
#include <base/KiplException.h>
#include <io/io_fits.h>
#include <io/DirAnalyzer.h>
#include <io/io_vivaseq.h>
#include <strings/filenames.h>

class kiplIOTest : public QObject
{
    Q_OBJECT

public:
    kiplIOTest();

private:
    void MakeFiles(std::string mask, int N, int first=0);

private Q_SLOTS:

    void testFITSreadwrite();
    void testCroppedFITSreading();
    void testFileItem();
    void testDirAnalyzer();
    void testCountLinesInFile();
    void testSEQHeader();
    void testSEQRead();
};

kiplIOTest::kiplIOTest()
{
}

void kiplIOTest::testFITSreadwrite()
{
    size_t dims[2]={10,12};
    kipl::base::TImage<short,2> img(dims);
    kipl::base::TImage<short,2> res;

    for (size_t i=0; i<img.Size(); ++i)
    {
            img[i]=i;
    }

    kipl::io::WriteFITS(img,"test.fits");

    kipl::io::ReadFITS(res,"test.fits");


    QVERIFY2(img.Size(0)==res.Size(0), "dim0 miss match");
    QVERIFY2(img.Size(1)==res.Size(1), "dim1 miss match");

    for (size_t i=0; i<img.Size(); ++i)
        QVERIFY2(img[i]==res[i], "value error");
}

void kiplIOTest::testCroppedFITSreading()
{

    QVERIFY2(true, "Failure");
}

void kiplIOTest::testFileItem()
{
    kipl::io::FileItem fi;

    QVERIFY(fi.m_nIndex==0);
    QVERIFY(fi.m_sExt=="tif");
    QVERIFY(fi.m_sMask=="noname_####.tif");
    QVERIFY(fi.m_sPath=="");

    kipl::io::FileItem fi2("hepp_####.abc",1,"abc","/home/data/");
    QVERIFY(fi2.m_nIndex==1);
    QVERIFY(fi2.m_sExt=="abc");
    QVERIFY(fi2.m_sMask=="hepp_####.abc");


    fi=fi2;

    QVERIFY(fi2.m_nIndex==fi.m_nIndex);
    QVERIFY(fi2.m_sExt==fi.m_sExt);
    QVERIFY(fi2.m_sMask==fi.m_sMask);
    QVERIFY(fi2.m_sPath==fi.m_sPath);
}

void kiplIOTest::testDirAnalyzer()
{
    MakeFiles("test_####.fits",5,1);
    MakeFiles("test2_#####.fits",3,6);

    kipl::io::DirAnalyzer di;

    kipl::io::FileItem fi=di.GetFileMask("hepp_1234.xyz");

    QVERIFY(fi.m_nIndex==1234);
    QVERIFY(fi.m_sExt=="xyz");
    QVERIFY(fi.m_sMask=="hepp_####.xyz");
    QVERIFY(fi.m_sPath=="");

    kipl::io::FileItem fi2=di.GetFileMask("hopp2_56780.abc");

    QVERIFY(fi2.m_nIndex==56780);
    QVERIFY(fi2.m_sExt=="abc");
    QVERIFY(fi2.m_sMask=="hopp2_#####.abc");
    QVERIFY(fi2.m_sPath=="");

    kipl::io::FileItem fi3=di.GetFileMask("hopp567.xyz");

    QVERIFY(fi3.m_nIndex==567);
    QVERIFY(fi3.m_sExt=="xyz");
    QVERIFY(fi3.m_sMask=="hopp###.xyz");
    QVERIFY(fi3.m_sPath=="");

    std::string fname="/home/data/hopp_567.xyz";
    kipl::strings::filenames::CheckPathSlashes(fname,false);
    kipl::io::FileItem fi4=di.GetFileMask(fname);

    QVERIFY(fi4.m_nIndex==567);
    QVERIFY(fi4.m_sExt=="xyz");
    QVERIFY(fi4.m_sMask=="/home/data/hopp_###.xyz");
    std::cout<<fi4.m_sPath<<std::endl;
    QVERIFY(fi4.m_sPath=="/home/data/");

//    std::string path="\\P08062_wood\\raw_CCD";

    std::string path="/";
    std::vector<std::string> dirlist=di.GetDirList(path);

    int cnt,first,last;


    QDir dir("/");

    QStringList qdirlist=dir.entryList(QDir::AllEntries | QDir::Hidden);
    std::ostringstream msg;
    msg<<"di size="<<dirlist.size()<<", qdir size="<<qdirlist.size();

    QVERIFY2(qdirlist.size()==static_cast<int>(dirlist.size()), msg.str().c_str());

    for (auto it=dirlist.begin(); it!=dirlist.end(); ++it)
        QVERIFY(qdirlist.contains(QString::fromStdString(*it)));


    di.AnalyzeMatchingNames("test_####.fits",cnt,first,last);
    QVERIFY(cnt==5);
    QVERIFY(first==1);
    QVERIFY(last==5);

    di.AnalyzeMatchingNames("test2_#####.fits",cnt,first,last);
    QVERIFY(cnt==3);
    QVERIFY(first==6);
    QVERIFY(last==8);
}

void kiplIOTest::MakeFiles(std::string mask, int N, int first)
{
    QDir dir;
    if (dir.exists("data")==false) {
        dir.mkdir("data");
    }

//    dir.setCurrent("data");

    std::string fname,ext;
    for (int i=0; i<N; ++i) {
        kipl::strings::filenames::MakeFileName(mask,i+first,fname,ext,'#');
        QFile f(QString::fromStdString(fname));
        f.open(QIODevice::WriteOnly);

        f.write(fname.c_str(),fname.size());

        f.close();
    }
//    dir.setCurrent("../");

}

void kiplIOTest::testCountLinesInFile()
{
    std::string fname="listfile.txt";
    QDir d;
    d.remove(QString::fromStdString(fname));
    std::ofstream outfile(fname.c_str());

    int N=5;
    for (int i=0; i<N; i++)
    {
        outfile<<i<<", text"<<i<<".fits"<<std::endl;
    }

    kipl::io::DirAnalyzer da;

    int cnt=0;
    da.AnalyzeFileList(fname,cnt);

    QVERIFY(N==cnt);
}


void kiplIOTest::testSEQHeader()
{
   std::ostringstream msg;
   kipl::io::ViVaSEQHeader header;

   msg<<"sizeof(header)=="<<sizeof(header);
   QVERIFY2(sizeof(header)==2048,msg.str().c_str());

   kipl::io::GetViVaSEQHeader("/Users/kaestner/Desktop/Video1.seq",&header);
//   std::cout << header.headerSize  << ", "
//             << header.imageWidth  << ", "
//             << header.imageHeight << ", "
//             << header.numberOfFrames<<std::endl;
   size_t dims[2]={0,0};
   int numframes=0;
   kipl::io::GetViVaSEQDims("/Users/kaestner/Desktop/Video1.seq",dims,numframes);

   QVERIFY(dims[0]==header.imageWidth);
   QVERIFY(dims[1]==header.imageHeight);
   QVERIFY(numframes==static_cast<int>(header.numberOfFrames));
}

void kiplIOTest::testSEQRead()
{
    kipl::base::TImage<float,3> img;
    kipl::io::ViVaSEQHeader header;

    kipl::io::GetViVaSEQHeader("/Users/kaestner/Desktop/Video1.seq",&header);
    kipl::io::ReadViVaSEQ("/Users/kaestner/Desktop/Video1.seq",img);

    QVERIFY(img.Size(0)==header.imageWidth);
    QVERIFY(img.Size(1)==header.imageHeight);
    QVERIFY(img.Size(2)==header.numberOfFrames);

    size_t roi[]={100,100,300,200};

    kipl::io::ReadViVaSEQ("/Users/kaestner/Desktop/Video1.seq",img,roi);

    QVERIFY(img.Size(0)==(roi[2]-roi[0]));
    QVERIFY(img.Size(1)==(roi[3]-roi[1]));
    QVERIFY(img.Size(2)==header.numberOfFrames);


}

QTEST_APPLESS_MAIN(kiplIOTest)

#include "tst_kipliotest.moc"
