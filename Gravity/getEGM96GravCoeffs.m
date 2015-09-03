function [C,S,a,c,CStdDev,SStdDev]=getEGM96GravCoeffs(M,isTideFree,Jul1,Jul2)
%%GETEGM96GRAVCOEFFS Get fully normalized spherical harmonic coefficients
%                    for the National Geospatial Intelligence Agency's
%                    (NGA's) Earth Gravitation Model 1996 (EGM96) up to
%                    a desired order. The coefficients can be for the
%                    zero-tide or the tide-free models. Note that
%                    time-varying components (e.g. the effects of the
%                    pull of the moon and other celestial bodies on the
%                    Earth) are not included and must be added
%                    separately. However, annual drift terms for some of
%                    the low order coefficients can be taken into account,
%                    if desired. Such terms were ommited in the EGM2008
%                    model.
%
%INPUTS:    M       The integer maximum order of the spherical harmonic
%                   coefficientsobtained. This is a value between 2 and
%                   360. If this parameter is omitted, the default value
%                   is 360.
%       isTideFree  A boolean value indicating whether the coefficients
%                   should be for a theoretical tide-free Earth or for a
%                   zero-tide model. If this parameter is omitted, the
%                   default value is isTideFree=true;
%      Jul1, Jul2   Two parts of a pseudo-Julian date given in UTC.
%                   The units of the date are days. The full date is
%                   the sum of both terms. The date is broken into two
%                   parts to provide more bits of precision. It does
%                   not matter how the date is split. If the date is
%                   omitted, then the coefficients for the epoch date of
%                   1 January 1986 will be used.
%
%OUTPUTS:   C   A ClusterSet class holding the coefficient terms that are
%               multiplied by cosines in the harmonic expansion. C(n+1,m+1)
%               is the coefficient of degree n and order m. When a maximum
%               degree of M is used, all C have values for all n from 0 to
%               M and for all m from 0 to n for each n. The coefficients
%               are unitless.
%           S   A ClusterSet class holding the coefficient terms that are
%               multiplied by sines in the harmonic expansion. The
%               format of S is the as that of C.
%           a   The numerator in the (a/r)^n term in the spherical harmonic
%               sum, having units of meters.
%           c   The constant value by which the spherical harmonic series
%               is multiplied, having units of m^3/s^2.
%       CStdDev The standard deviation of each C coefficient.
%       SSTdDev The standard deviation of each S coefficient.
%
%This function first checks for a .mat file with the coefficients in it.
%The .mat file is small and can be read quite quickly. However, if one does
%not exist, then it tries to read the egm96_to360.ascii text file
%that one can obtain directly from the NGA. Reading from the text file is
%extremely slow.
%
%More on using the spherical harmonic coefficients is given in
%the comments for the function spherHarmonicEval and the format and use of
%the coefficients is also documented in
%http://cddis.gsfc.nasa.gov/926/egm96/egm96.html
%http://earth-info.nga.mil/GandG/wgs84/gravitymod/egm96/egm96.html
%
%Despite being outmoded by the EGM2008 model, the EGM96 coefficients remain
%used for defining the geoid used for the vertical datum with lots of
%digital terrain elevation models.
%
%January 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.
%(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.

if(nargin<1)
    M=360;
end

if(nargin<2)
    isTideFree=true;
end

if(M>360)
    error('The EGM96 model only has coefficients up to degree 360') 
end

%If a date was given, convert the date to an offset in years from the epoch
%date of January 1st 1986.
if(nargin>2)
    [Jul1Epoch,Jul2Epoch]=Cal2UTC(1986,1,1,0,0,0);
    %The loss of precision does not matter due to the infidelity of the
    %model. This ignores leap seconds.
    deltaJulDays=Jul1Epoch+Jul2Epoch-(Jul1+Jul2);
    %The loss of precision in ignoring leap years and leap seconds and just
    %using an average year duration shouldn't matter due to the precision
    %of the model's drift coefficients. A Julian year has 365.25 days.
    deltaJulYears=deltaJulDays/365.25;
end

a=Constants.EGM2008SemiMajorAxis;
c=Constants.EGM2008GM;

totalNumCoeffs=(M+1)*(M+2)/2;

%The EGM20008 coefficient data file, should be located in a data folder 
%that is in the same folder as this file. This find the path to this file. 
ScriptPath=mfilename('fullpath');
ScriptFolder = fileparts(ScriptPath);

%First, see if a .mat file with all of the data exists. If so, then use
%that and ignore everything else.
if(exist([ScriptFolder,'/data/EGM96Coeffs.mat'],'file'))
    load([ScriptFolder,'/data/EGM96Coeffs.mat'],'CCoeffs','SCoeffs','CCoeffsStdDev','SCoeffsStdDev','clustSizes','offsets');
    %Create the ClusterSet classes to hold the data.
    C=ClusterSet();
    S=ClusterSet();
    CStdDev=ClusterSet();
    SStdDev=ClusterSet();
    
    C.clusterEls=CCoeffs(1:totalNumCoeffs);
    S.clusterEls=SCoeffs(1:totalNumCoeffs);
    CStdDev.clusterEls=CCoeffsStdDev(1:totalNumCoeffs);
    SStdDev.clusterEls=SCoeffsStdDev(1:totalNumCoeffs);
    
    C.clusterSizes=clustSizes(1:(M+1));
    S.clusterSizes=C.clusterSizes;
    CStdDev.clusterSizes=C.clusterSizes;
    SStdDev.clusterSizes=C.clusterSizes;
    
    C.offsetArray=offsets(1:(M+1));
    S.offsetArray=C.offsetArray;
    CStdDev.offsetArray=C.offsetArray;
    SStdDev.offsetArray=C.offsetArray;
    
    %If a date other than the epoch should be used.
    if(nargin>2)
        C(2+1,0+1)=C(2+1,0+1)+deltaJulYears*Constants.EGM96C20BarDot;
        C(2+1,1+1)=C(2+1,1+1)+deltaJulYears*Constants.EGM96C21BarDot;
        S(2+1,1+1)=S(2+1,1+1)+deltaJulYears*Constants.EGM96S21BarDot;
    end
    
    %If the zero-tide model should be used.
    if(isTideFree==false)
        %The zero-tide model differs by only the C_{2,0} coefficient.
        C(2+1,0+1)=C(2+1,0+1)- 3.11080e-8*0.3/sqrt(5);
    end
    return 
end

%If the .mat file does not exist, then assume that the coefficients must be
%read from the text file provided by the NGA.

%Allocate space for the coefficients.
emptyData=zeros(totalNumCoeffs,1);
clustSizes=1:(M+1);
C=ClusterSet(emptyData,clustSizes);
S=ClusterSet(emptyData,clustSizes);
CStdDev=ClusterSet(emptyData,clustSizes);
SStdDev=ClusterSet(emptyData,clustSizes);

%Read in the data up to the specified order. The -3 deals with the fact
%that coefficients for degrees and order (0,0), (1,0) and (1,1) have been
%omitted from the data file.
fileID=fopen([ScriptFolder,'/data/egm96_to360.ascii']);
data=textscan(fileID,'%d %d %f %f %f %f',totalNumCoeffs-3);
fclose(fileID);

%Put the data into the ClusterSet class instances.
numRows=totalNumCoeffs-3;
for curRow=1:numRows
    n=data{1}(curRow);%The degree of the coefficient.
    m=data{2}(curRow);%The order of the coefficient.
    C(n+1,m+1)=data{3}(curRow);
    S(n+1,m+1)=data{4}(curRow);
    CStdDev(n+1,m+1)=data{5}(curRow);
    SStdDev(n+1,m+1)=data{6}(curRow);
end

%The EGM96 coefficient file omits the C_{0,0} value. It is explicitely
%added here.
C(0+1,0+1)=1;

%If a date other than the epoch should be used.
if(nargin>2)
    C(2+1,0+1)=C(2+1,0+1)+deltaJulYears*Constants.EGM96C20BarDot;
    C(2+1,1+1)=C(2+1,1+1)+deltaJulYears*Constants.EGM96C21BarDot;
    S(2+1,1+1)=S(2+1,1+1)+deltaJulYears*Constants.EGM96S21BarDot;
end

%If the zero-tide model should be used.
if(isTideFree==false)
    %The zero-tide model differs by only the C_{2,0} coefficient.
    C(2+1,0+1)=C(2+1,0+1)-3.11080e-8*0.3/sqrt(5);
end
end

%LICENSE:
%
%The source code is in the public domain and not licensed or under
%copyright. The information and software may be used freely by the public.
%As required by 17 U.S.C. 403, third parties producing copyrighted works
%consisting predominantly of the material produced by U.S. government
%agencies must provide notice with such work(s) identifying the U.S.
%Government material incorporated and stating that such material is not
%subject to copyright protection.
%
%Derived works shall not identify themselves in a manner that implies an
%endorsement by or an affiliation with the Naval Research Laboratory.
%
%RECIPIENT BEARS ALL RISK RELATING TO QUALITY AND PERFORMANCE OF THE
%SOFTWARE AND ANY RELATED MATERIALS, AND AGREES TO INDEMNIFY THE NAVAL
%RESEARCH LABORATORY FOR ALL THIRD-PARTY CLAIMS RESULTING FROM THE ACTIONS
%OF RECIPIENT IN THE USE OF THE SOFTWARE.
