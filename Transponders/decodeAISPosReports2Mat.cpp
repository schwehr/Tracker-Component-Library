/*DECODEAISPOSREPORTS2MAT Given a cell array of Automatic Identification
 *                 System (AIS) message given as National Marine
 *                 Electronics Association (NMEA) formatted ASCII text
 *                 messages, extract poistions reports of message types 1,
 *                 2, 3, 18, and 27, storing the information related to
 *                 ship location, identity, course, and time in different
 *                 rows of a matrix, where each column corresponds to a
 *                 different report. It is assumed that the device
 *                 recording the messages has already removed duplicate
 *                 messages arising from messages being repeated.
 *                 It is also possible to decode timestamps appended as a
 *                 final field on the messages in seconds since an epoch
 *                 date. The manipulation of matrices rather than
 *                 structures is significantly faster in Matlab, which is
 *                 why one might prefer this function to decodeAISString
 *                 when only simple position reports are needed.
 *
 *INPUTS: NMEAStrings A cell array contianing multiple full AIS NMEA
 *                    messages. Invalid messages and messages that are not
 *                    of types 1, 2, 3, 18, and 27 are skipped.
 *     convert2Metric If true, all distance units in the result are
 *                    converted to meters, all speed units are converted to
 *                    meters per second, and all angles are converted to
 *                    radians North of east (the mathematical definition)
 *                    and angular rates are converted to radians per second
 *                    North of East, all rather than using typical nautical
 *                    units.  The default if omitted is true.
 *
 *OUTPUTS: decodedMsgs An 11XN matrix of the N successfully decoded
 *                     position reports. If the AIS report contained a
 *                     value corresponding to a parameter being undefined
 *                     or not set, then that value is replaced by a NaN in
 *                     the matrix. The rows of the matrix correspond to:
 *                     1) The maritime mobile service identity (MMSI)
 *                        number of the transmitter that can be used to
 *                        identify targets (A 30-bit integer value).
 *                     2) Navigation status. An integer indicating what the
 *                        ship is doing/ whether it is moving. Possible
 *                        valid values are:
 *                        *0 = under way using engine
 *                        *1 = at anchor
 *                        *2 = not under command,
 *                        *3 = restricted maneuverability
 *                        *4 = constrained by her draught
 *                        *5 = moored
 *                        *6 = aground
 *                        *7 = engaged in fishing
 *                        *8 = under way sailing
 *                        *9 = reserved for future amendment of navigational status for ships carrying DG, HS, or MP, or IMO hazard or pollutant category C, high speed craft (HSC)
 *                        *10 = reserved for future amendment of navigational status for ships carrying dangerous goods (DG), harmful substances (HS) or marine pollutants (MP), or IMO hazard or pollutant category A, wing in ground (WIG)
 *                        *11 = power- driven vessel towing astern (regional use)
 *                        *12 = power-driven vessel pushing ahead or towing alongside (regional use)
 *                        *13 = reserved for future use,
 *                        *14 = AIS-SART (active), MOB-AIS, EPIRB-AIS
 *                     3) Turn rate. This is limited to +/- 708 degrees per
 *                        minute (about +/- .2059 radians per second when
 *                        using metric units).
 *                     4) Speed over ground in knots up to 102.2 knots, or
 *                        52.6 meters per second when using metric units.
 *                     5) Position accuracy flag. Possible values are:
 *                        1=high (accuracy beter than 10m)
 *                        0=low (accuracy worse than 10 meters)
 *                     6) Latitude (WGS-84) in degrees North or radians
 *                        North when using the metric units option.
 *                     7) Longitude (WGS 84) in degrees East or radians
 *                        East when using the metric units option.
 *                     8) Course over ground (heading) in degrees East of
 *                        North or radians North of East when using the
 *                        metric units option.
 *                     9) True heading in degrees East of North or radians
 *                        North of East when using the metric units option.
 *                     10)The second number within the UTC minute when
 *                        the ship transmitter sent the position report.
 *                     11)Special manoeuvre indicator. This can be
 *                        1 = not engaged in special manoeuvre
 *                        2 = engaged in special manoeuvre
 *          epochTimes If this parameter is requested, then this indicates
 *                     that the final field in the NMEA strings is be a
 *                     timestamp in UTC/ TT seconds since some epoch date
 *                     and that it should be decoded. If this parameter is
 *                     not requested, then no attempt is made to decode
 *                     timestamps appended by a receiver.
 *
 *Ship reports that mark the positioning system as being broken or in dead
 *reckoning mode are discarded. However, reports indicating the ship is in
 *manual mode are retained. Ship reports that indicate the position
 *measurement is delayed more than 5 seconds are also discarded. This only
 *occurs in message 27. Ship reports in which latitude or longitude are
 *unavailable are also discarded. AIS message 19 can also report a ship's
 *position. However, the ITU-R M.1371-5 recommends that it not be used, so
 *it is unlikely to be seen in the future. Moreover, it is inconvenient to
 *decode, because it is a two-part message.
 *
 *More information on AIS standards is given in the comments to the
 *decodeAISString function, which can decode a much wider variety of AIS
 *messages into structures, but which is slower.
 *
 *This function relies on libais from 
 *https://github.com/schwehr/libais
 *The library probably does not work on big-endian processors (Intel
 *processors are little-endian).
 *
 *The algorithm can be compiled for use in Matlab using the 
 *CompileCLibraries function.
 *
 *The algorithm is run in Matlab using the command format
 *decodedMsgs=decodeAISPosReports2Mat(NMEAStrings,convert2Metric);
 *or
 *[decodedMsgs,epochTimes]=decodeAISPosReports2Mat(NMEAStrings,convert2Metric);
 *if timestamps are available
 *
 *November 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.*/
/*(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.*/

//This definition is necessary or else the aislib files will cause
//assertions when bad messages are passed.
#ifndef NDEBUG
#define NDEBUG
#endif

//Needed to get a NaN value.
#include <limits>
//For strtoul
#include <cstdlib>
/*This header is required by Matlab.*/
#include "mex.h"
#include "ais.h"
#include "MexValidation.h"
//For abs
#include <cmath>

//Multiple a value in degrees by deg2Rad to get its value in radians.
const double deg2Rad=0.0174532925199432957692369076849;
const double halfPi=1.57079632679489661923132169164;
const double min2Sec=60;
//Conversion of knots to meters per hour.
const double knot2Metph=1852;
const double hour2Sec=60*60;

//Prototype declaration
bool NMEAChecksumValid(const char * const theMessage);

using namespace std;
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) { 
    bool decodeTimestamps=false;
    bool convert2Metric=true;
    size_t numCells;
    mxArray *retMat, *timestampMat;
    double *retData, *timestampData;
    size_t i;
    size_t numDecoded;
    const size_t numRowsInOutput=11;
    
    if(nrhs!=1&&nrhs>2) {
        mexErrMsgTxt("Wrong number of inputs.");
        return;
    }

    if(nlhs>1) {
        decodeTimestamps=true;
    }
    
    if(nrhs>1) {
        convert2Metric=getBoolFromMatlab(prhs[1]);
    }
            
    if(nlhs>2) {
        mexErrMsgTxt("Wrong number of outputs.");
        return;
    }
    
    if(mxGetNumberOfDimensions(prhs[0])>2||mxGetNumberOfDimensions(prhs[0])<1) {
        mexErrMsgTxt("The input has an invalid dimensionality.");
        return;
    }
    {
        const size_t numRows=mxGetM(prhs[0]);
        const size_t numCols=mxGetN(prhs[0]);
        //If a 2D cell array is passed, then it is unclear which order the
        //messages come in, so we will just return an error.
        if(mxIsEmpty(prhs[0])||(numRows!=1&&numRows!=1)) {
            mexErrMsgTxt("A cell array that is passed must be 1-dimensional.");
        }
        numCells=max(numRows,numCols);
    }

    //Allocate the maximum necessary space for the return data.
    retMat=mxCreateDoubleMatrix(numRowsInOutput,numCells,mxREAL);
    retData=(double*)mxGetData(retMat);
    plhs[0]=retMat;//Set the return matrix.
    if(decodeTimestamps) {
        timestampMat=mxCreateDoubleMatrix(numCells,1,mxREAL);
        timestampData=(double*)mxGetData(timestampMat);
        plhs[1]=timestampMat;//Set the return matrix.
    }
    
    //This function must be called before any AIS messages can be
    //decoded.
    BuildNmeaLookup();
    
    numDecoded=0;
    for(i=0;i<numCells;i++) {
        int numPadBits;
        const mxArray *curCell=mxGetCell(prhs[0], i);
        char *msgChars;
        const char *bodyChars;
        string theCellString, msgBody;
        int message_id;
        AIS_STATUS r;
        const size_t outputOffset=numRowsInOutput*numDecoded;
        
        if(!mxIsChar(curCell)||mxIsEmpty(curCell)) {
            continue;//Skip invalid entries.
        }
        
        msgChars=mxArrayToString(curCell);
        theCellString=string(msgChars);
        numPadBits=GetPad(theCellString);
        
        if(!NMEAChecksumValid(msgChars)){
            mxFree(msgChars);
            continue;//Skip invalid checksums   
        } else if(numPadBits<0) {
            mxFree(msgChars);
            continue;//Skip bad formatting to the checksum
        }
        
        //Make sure that it is a valid single-part message
        {
            size_t j;
            bool isInvalid=false;
            for(j=1;j<=2;j++) {
                size_t theNum;
                const string tempString=GetNthField(theCellString,j,string(","));
                //If an error occurred extracting the first message number 
                if(tempString.empty()) {
                    mxFree(msgChars);
                    continue;//Cannot extract first number field
                }
                theNum=strtoul(tempString.c_str(),NULL,10);
                //It must be one field out of a total of one fields.
                if(theNum!=1) {
                    isInvalid=true;
                    break;
                }
            }
            if(isInvalid) {
                mxFree(msgChars);
                continue;
            }
        }
        
        //Extract the data portion of the message.
        msgBody=GetBody(theCellString);
        //Get the message characters to decode
        bodyChars=msgBody.c_str();
        if(msgBody.size()<7) {//Not enough characters for the header.
            mxFree(msgChars);
            continue;
        }
        
        //Decode the Message ID
        {
            const string messageHeader(bodyChars, 7);
            const size_t numBits = msgBody.size()*6-numPadBits;
            //This is space for the message header after being turned into a bunch
            //of bits. It is 7 characters at 6 bits each.
            bitset<42> bs;
            
            //Make sure that enough bits for the header are present.
            if (numBits < 38) {
                mxFree(msgChars);
                continue;
            }
            r=aivdm_to_bits(bs, messageHeader.c_str());
            //If the message header could not be turned into bits.
            if(r!=AIS_OK) {
                mxFree(msgChars);
                continue;
            }
            message_id = ubits(bs, 0, 6);
        }
        
        switch(message_id) {
            case 1:
            case 2:
            case 3:
            {   
                Ais1_2_3 msg(bodyChars, numPadBits);
                if (msg.had_error()) {
                    mxFree(msgChars);
                    continue;
                }
                
                //A timestamp over 61 indicates that the positioning system
                //is broken or in dead reckoning mode, so the return is
                //likely to be inaccurate. Also, if latitude or longitude
                //are not available the return is also discarded.
                if(msg.timestamp>61||fabs(msg.y)>90||fabs(msg.x)>180) {
                    mxFree(msgChars);
                    continue;
                }
                
                retData[outputOffset+0]=(double)msg.mmsi;
                retData[outputOffset+1]=(double)msg.nav_status;
                if(!msg.rot_over_range) {
                    retData[outputOffset+2]=msg.rot;
                } else {
                    retData[outputOffset+2]=numeric_limits<double>::quiet_NaN();
                }
                
                if(msg.sog<=102.2) {
                    retData[outputOffset+3]=msg.sog;
                } else {
                    retData[outputOffset+3]=numeric_limits<double>::quiet_NaN();
                }
                
                retData[outputOffset+4]=(double)msg.position_accuracy;

                //Latitude
                retData[outputOffset+5]=msg.y;
                
                //Longitude
                retData[outputOffset+6]=msg.x;
                
                if(msg.cog<360) {
                    retData[outputOffset+7]=msg.cog;
                } else {
                    retData[outputOffset+7]=numeric_limits<double>::quiet_NaN();
                }
                
                if(msg.true_heading<360) {
                    retData[outputOffset+8]=(double)msg.true_heading;
                } else {
                    retData[outputOffset+8]=numeric_limits<double>::quiet_NaN();
                }
                
                //The restriction against  it being 60 means that an
                //incorrect UTC second will be reported when a leapsecond
                //is added.
                if(msg.timestamp<60) {
                    retData[outputOffset+9]=(double)msg.timestamp;
                } else {
                    retData[outputOffset+9]=numeric_limits<double>::quiet_NaN();
                }
                if(msg.special_manoeuvre!=0&&msg.special_manoeuvre<3) {
                    retData[outputOffset+10]=msg.special_manoeuvre;
                } else {
                    retData[outputOffset+10]=numeric_limits<double>::quiet_NaN();
                }
            }
                break;
            case 18:
            {
                Ais18 msg(bodyChars, numPadBits);
                if (msg.had_error()) {
                    mxFree(msgChars);
                    continue;
                }
                
                //A timestamp over 61 indicates that the positioning system
                //is broken or in dead reckoning mode, so the return is
                //likely to be inaccurate. Also, is latitude or longitude
                //are not available the return is also discarded.
                if(msg.timestamp>61||fabs(msg.y)>90||fabs(msg.x)>180) {
                    mxFree(msgChars);
                    continue;
                }
                
                retData[outputOffset+0]=(double)msg.mmsi;
                //There is no navigation status
                retData[outputOffset+1]=numeric_limits<double>::quiet_NaN();
                //There is no turn rate
                retData[outputOffset+2]=numeric_limits<double>::quiet_NaN();
                
                if(msg.sog<=102.2) {
                    retData[outputOffset+3]=msg.sog;
                } else {
                    retData[outputOffset+3]=numeric_limits<double>::quiet_NaN();
                }
                
                retData[outputOffset+4]=(double)msg.position_accuracy;

                //Latitude
                if(fabs(msg.y)<=90) {
                    retData[outputOffset+5]=msg.y;
                } else {
                    retData[outputOffset+5]=numeric_limits<double>::quiet_NaN();
                }
                
                //Longitude
                if(fabs(msg.x)<=180) {
                    retData[outputOffset+6]=msg.x;
                } else {
                    retData[outputOffset+6]=numeric_limits<double>::quiet_NaN();
                }
                
                if(msg.cog<360) {
                    retData[outputOffset+7]=msg.cog;
                } else {
                    retData[outputOffset+7]=numeric_limits<double>::quiet_NaN();
                }
                
                if(msg.true_heading<360) {
                    retData[outputOffset+8]=(double)msg.true_heading;
                } else {
                    retData[outputOffset+8]=numeric_limits<double>::quiet_NaN();
                }
                
                //The restriction against it being 60 means that an
                //incorrect UTC second will be reported when a leapsecond
                //is added.
                if(msg.timestamp<60) {
                    retData[outputOffset+9]=(double)msg.timestamp;
                } else {
                    retData[outputOffset+9]=numeric_limits<double>::quiet_NaN();
                }
                //There is no special manoeuvre.
                retData[outputOffset+10]=numeric_limits<double>::quiet_NaN();
            }
                break;
            case 27:
            {
                Ais27 msg(bodyChars, numPadBits);
                if (msg.had_error()) {
                    mxFree(msgChars);
                    continue;
                }
                
                //If the measurements are not accurate (possibly delayed
                //as per the GNSS flag), then simply omit them. Also, if
                //latitude or longitude are not available the return is
                //discarded.
                if(!msg.gnss||fabs(msg.y)>90||fabs(msg.x)>180) {
                    mxFree(msgChars);
                    continue;
                }
                
                retData[outputOffset+0]=(double)msg.mmsi;
                retData[outputOffset+1]=(double)msg.nav_status;
                //There is no turn rate information
                retData[outputOffset+2]=numeric_limits<double>::quiet_NaN();
                
                if(msg.sog<=102.2) {
                    retData[outputOffset+3]=msg.sog;
                } else {
                    retData[outputOffset+3]=numeric_limits<double>::quiet_NaN();
                }
                
                retData[outputOffset+4]=(double)msg.position_accuracy;

                //Latitude
                retData[outputOffset+5]=msg.y;
                
                //Longitude
                retData[outputOffset+6]=msg.x;
                
                if(msg.cog<360) {
                    retData[outputOffset+7]=msg.cog;
                } else {
                    retData[outputOffset+7]=numeric_limits<double>::quiet_NaN();
                }
                
                //There is no true heading information
                retData[outputOffset+8]=numeric_limits<double>::quiet_NaN();
                
                //There is no UTC seconds timestamp in Message 27
                retData[outputOffset+9]=numeric_limits<double>::quiet_NaN();
                //There is no special manoeuvre information
                retData[outputOffset+10]=numeric_limits<double>::quiet_NaN();
            }
                break;
            default:
                //Unknown message type or it is not a position report
                //supported by this function.
                continue;
        }
        //If it is here, then the message was successfully decoded and we
        //can consider the timestamps and whether the values should be
        //converted to metric units.
        if(decodeTimestamps) {
            //Find the last comma in the string.
            const size_t lastCommaIdx=theCellString.find_last_of(",");
            size_t theTimestamp;
            //Everything from after the last comma until the end of the
            //string should be a timestamp.
            theTimestamp=strtoul(msgChars+lastCommaIdx+1,NULL,10);
            //If a bunch of non-numeric characters were passed, strtoul
            //will return zero and not throw and error. Thus, we have to
            //scan the string to see if there should be an error if a zero
            //value is returned.
            
            if(theTimestamp==0) {
                size_t curIdx=lastCommaIdx+1;
                if(msgChars[curIdx]=='\0'||!isdigit(msgChars[curIdx])) {
                    //If there is nothing after the comma or the first
                    //character is not numeric, then mark the timestamp as
                    //invalid.
                    timestampData[numDecoded]=numeric_limits<double>::quiet_NaN();
                } else {
                    //Now, scan until the end of the string or until the
                    //first non-numeric character.
                    for(curIdx++;msgChars[curIdx]!='\0'&&isdigit(msgChars[curIdx]);curIdx++);
                    //If it was successfully parsed
                    if(msgChars[curIdx]=='\0') {
                        timestampData[numDecoded]=(double)theTimestamp;
                    } else {
                        timestampData[numDecoded]=numeric_limits<double>::quiet_NaN();
                    }
                }
            } else {
                timestampData[numDecoded]=(double)theTimestamp;
            }
        }
        mxFree(msgChars);
        
        if(convert2Metric) {
            //Convert turn rate from degrees per minute East of North to
            //radians per second North of East
            retData[outputOffset+2]=-deg2Rad*retData[outputOffset+2]/min2Sec;                    
                    
            //Convert speed over ground from knots to meters per second.
            retData[outputOffset+3]=retData[outputOffset+3]*knot2Metph/hour2Sec;
            //Convert latitude to radians
            retData[outputOffset+5]=deg2Rad*retData[outputOffset+5];
                
            //Convert longitude to radians
            retData[outputOffset+6]=deg2Rad*retData[outputOffset+6];
            
            //Convert course over ground from degrees East of North to
            //radians North of East
            retData[outputOffset+7]=halfPi-deg2Rad*retData[outputOffset+7];
            //Convert true heading from degrees East of North to radians
            //North of East
            retData[outputOffset+8]=halfPi-deg2Rad*retData[outputOffset+8];
        }
        numDecoded++;
    }
    //Reshape the output arrays to match the actual number of position
    //reports decoded.
    {
        const mwSize dimsData[2]={numRowsInOutput,numDecoded};
        mxSetDimensions(retMat,dimsData,2);
        if(decodeTimestamps) {
            const mwSize dimsTimestamps[2]={numDecoded,1};
            mxSetDimensions(timestampMat,dimsTimestamps,2);
        }
    }
}

bool NMEAChecksumValid(const char * const theMessage) {
//The checksum as well as the basic format have to both be valid for the
//NMEA message. 
    int checksum=0;
    char hexString[3];//Space for two characters and a null termination.
    size_t i;
    
    //Valid strings begins with ! or $.
    if(theMessage[0]!='!'&&theMessage[0]!='$' ) {
        return false;
    }
    
    i=1;
    //The string to checksum ends at the *
    while(theMessage[i]!='\0'&&theMessage[i]!='*') {
        checksum=checksum^theMessage[i];
        i++;
    }
    
    //If it got to the end of the string before reaching to '*' character
    if(theMessage[i]!='*') {
        return false;
    }

    //Convert the checksum into a two-digit hexadecimal string.
    sprintf(hexString, "%02X", checksum);
    
    //Make sure that the message has two additional values so that the
    //checksum can be verified and that anything after comes after a comma.
    i++;
    if(theMessage[i]=='\0'||theMessage[i+1]=='\0') {
        return false;
    }
    if(theMessage[i+2]!='\0'&&theMessage[i+2]!=',') {
        return false;
    }
    {//Verify checksum
        const char checksumString[3]={theMessage[i],theMessage[i+1],'\0'};
        if(strcmp(hexString,checksumString)==0) {
            return true;
        } else {
            return false;
        } 
    }
}

/*LICENSE:
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
%OF RECIPIENT IN THE USE OF THE SOFTWARE.*/
