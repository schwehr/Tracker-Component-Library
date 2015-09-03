/**DECODEAISSTRING Decode an Automatic Identification System (AIS)
 *                 message given as a string of characters. This can be a
 *                 full National Marine Electronics Association (NMEA)
 *                 formatted ASCII text message, just the payload
 *                 characters of such a message, or a cell array of
 *                 messages, whereby multiple fragment messages are pieced
 *                 together. If a cell array of messages is provided, the
 *                 first message is decoded and the number of fragments
 *                 combined for the message is given allowing one to call
 *                 this function multiple times, eliminating the first few
 *                 entries of the cell array each time, to decode a large
 *                 set of messages.
 *
 *INPUTS:   NMEAStrings A character string of a single NMEA message, either
 *                      as a full message or just the data payload, or a
 *                      one-dimensional cell array containing multiple full
 *                      NMEA messages.
 *          param2      The use of the second parameter depends on whether
 *                      a character string or a cell array is passed. If 
 *                      just a character string is passed, then param2 is 
 *                      numPadBits -This parameter is only needed if just
 *                      the payload is passed and not a full NMEA string,
 *                      which typically beings with !AIVDM. In such an
 *                      instance, this is the number of bits that were
 *                      included to pad the message to a 6-bit boundary.
 *                      If this is omitted, it is assume to be zero. If a
 *                      full NMEA message string is given, then this is not
 *                      used.
 *                      If a cell array is passed, then this is
 *                      decodeAll  -This boolean parameter is true if all
 *                      of the messages in the cell array should be decoded
 *                      with all of the results put into cell arrays.
 *                      Otherwise, just the first message is decoded. The
 *                      default if this parameter is omitted is false.
 *
 *OUTPUTS:decodedMessage A structure whose components are the decoded
 *                    message elements. If an incomplete or
 *                    invalid message is provided, an empty matrix will be
 *                    returned. If only a partially recognized message is
 *                    given, then only the known fields will be returned in
 *                    the structure. If NMEAStrings is a cell array and
 *                    param2 (decodeAll) is true, then this is a cell array
 *                    of decoded messages for all of the messages.
 *       numFragsUsed The number of NMEA messages that were combined to
 *                    form the decoded message or that were reviewed before
 *                    determining that the message is invalid/ cannot be
 *                    decoded. If NMEAStrings is a single character string,
 *                    this is 1. If NMEAStrings is a cell array, this is
 *                    the number of cells examined and determined to form a
 *                    valid message/ an invalid message or an incomplete
 *                    message fragment. If NMEAStrings is a cell array and
 *                    param2 (decodeAll) is true, then this is an array
 *                    of the number of fragments used for all of the
 *                    messages.
 *          errorCode A value indicating any type of decoding error that
 *                    might have occurred resulting in decodedMessage being
 *                    an empty matrix. Otherwise, this is just the string
 *                   'AIS-OK'. If NMEAStrings is a cell array and
 *                    param2 (decodeAll) is true, then this is a cell array
 *                    of the errorDesc values for for all of the
 *                    messages. Possible error codes are
 *                    0  The message decoding system was not properly
 *                       initialized (should not occur).
 *                    1  There was no error decoding the message.
 *                    2  Bad bit count.
 *                    3  Bad NMEA character.
 *                    4  Bad pointer (should not occur).
 *                    5  Unknown message type (should not occur, since
 *                       unknown message ID's are returned as a
 *                       decodedMessage contianing nothing but the message
 *                       ID).
 *                    6  Message not implemented (should not occur).
 *                    7  Message subtype not implemented (should not occur).
 *                    8  AIS_ERR_EXPECTED_STRING (should not occur).
 *                    9  Bad message content.
 *                    10 Message is too long.
 *                    11 Bad sub message.
 *                    12 Bad sub sub message.
 *                    13 Bad NMEA checksum.
 *                    14 Bad sequence of fragments for reconstructing
 *                       multipart messages.
 *
 *         reportName The name of the general type of message decoded.
 *                    If NMEAStrings is a cell array and param2 (decodeAll)
 *                    is true, then this is a cell array of the report
 *                    names for for all of the messages.
 *  reportDescription A string describing the type of message decoded. If
 *                    NMEAStrings is a cell array and param2 (decodeAll)
 *                    is true, then this is a cell array of the report
 *                    descriptions for for all of the messages.
 *  fieldDescriptions A structure array with the same general fields as
 *                    decodedMessage but where each field contains a string
 *                    describing what is in the field. If NMEAStrings is a
 *                    cell array and param2 (decodeAll) is true, then this
 *                    is a cell array of the field descriptions for all of
 *                    the messages.
 *
 *AIS messages are used for tracking cooperative ships and for the exchange
 *of information of interest to ships, such as tidal information. The basic
 *standard is
 *"Technical characteristics for an automatic identification system using
 * time division multiple access in the VHF maritime mobile frequency
 * band," International Telecommunication Union, Radiocommunication Sector
 * Std. ITU-R M.1371-5, Feb. 2014. [Online].
 * Available: http://www.itu.int/rec/R- REC- M.1371/en
 *with additional messages defined in
 *"Guidance on the use of AIS application-specific messages," International
 * Maritime Organization, 2 Jun. 2013. [Online].
 * Available: http://www.iho.int/mtg_docs/com_wg/IHOTC/IHOTC_Misc/IMO%20SN_Circ289.pdf
 *and in
 *"International standard for tracking and tracing on inland waterways
 * (VTT)," United Nations Economic and Social Council, Economic Commission
 * for Europe, Inland Transport Committee, New York and Geneva, 2007.
 * [Online]. Available: http://www.unece.org/fileadmin/DAM/trans/doc/finaldocs/sc3/ECE-TRANS-SC3-176e.pdf
 *Note that an older version of the ITU standard as well as
 *"Guidance on the application of AIS binary messages," International
 * Maritime Organization, 28 May 2004. [Online].
 * Available: http://www.imo.org/blast/blastDataHelper.asp?data id=10741&filename=236.pdf
 *contain some message types that have been removed from the newer
 *standards.
 *
 *A full NMEA message looks like
 *!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C
 *Just the payload of the message is
 *177KQJ5000G?tO`K>RA1wUbN0TKH
 *When decoding a single message given as a character string, then the full
 *NMEA message or just the payload can be used. When using a cell array of
 *messages, only full messages can be used. The two characters after the *
 *are the checksum. When using full messages, the decoding will fail if the
 *checksum is invalid. When using only partial messages, the numPadBits.
 *Newer NMEA messages can be preceded by tag blocks that begin with \. This
 *function assumes there are no tag blocks. Also, any data appended after a
 *comma after the checksum is ignored. Some receivers append data such as
 *times to the end. The NMEA message format is said to be described in the
 *NMEA 0183 Standard, which can be purchased from
 *http://www.nmea.org/content/nmea_standards/nmea_0183_v_410.asp
 *However, due to the high price of the standard, it was not consulted when
 *writing this function.
 *
 *This function relies on libais from 
 *https://github.com/schwehr/libais
 *The library does not support all possible message types. Also, the
 *library is a work in progress and does not decode all messages properly.
 *Messages that the library is known to incorrectly decode are treated as
 *unsupported messages. The library probably does not work on big-endian
 *processors (Intel processors are little-endian).
 *
 *The algorithm can be compiled for use in Matlab using the 
 *CompileCLibraries function.
 *
 *The algorithm is run in Matlab using the command format
 *[decodedMessage, numFragsUsed, errorCode,reportName,reportDescription,fieldDescriptions]=decodeAISString(NMEAStrings,numPadBits);
 *when given a character string that is just the payload of an NMEA
 *message, or as
 *[decodedMessage, numFragsUsed, errorCode,reportName,reportDescription,fieldDescriptions]=decodeAISString(NMEAStrings);
 *when given a character string that is a full NMEA message, or as
 *[decodedMessage, numFragsUsed, errorCode,reportName,reportDescription,fieldDescriptions]=decodeAISString(NMEAStrings,decodeAll);
 *when given a cell array.
 *
 *November 2014 David F. Crouse, Naval Research Laboratory, Washington D.C.*/
/*(UNCLASSIFIED) DISTRIBUTION STATEMENT A. Approved for public release.*/

//This definition is necessary or else the aislib files will cause
//assertions when bad messages are passed.
#ifndef NDEBUG
#define NDEBUG
#endif

/*This header is required by Matlab.*/
#include "mex.h"
#include "ais.h"
#include "ais8_001_22.h"
//For abs
#include <cmath>
//For strcpy
#include <cstring>
//For strtoul
#include <cstdlib>
//For strcmp
#include <cstdio>
//For max
#include <algorithm>
#include "MexValidation.h"

const size_t numReportTypes=28;

//The names from Table 46 in ITU-R M.1371-5 
const char *AISReportNames[numReportTypes] = {
    "",//There is no zero
    "Position Report",//1
    "Position Report",//2
    "Position Report",//3
    "Base Station Report",//4
    "Static and Voyage Related Data",//5
    "Binary Addressed Message",//6
    "Binary Acknowledgement",//7
    "Binary Broadcast Message",//8
    "Standard search and rescue (SAR) Aircraft Position Report",//9
    "UTC/Date Inquiry",//10
    "UTC/Date Response",//11
    "Addressed Safety Related Message",//12
    "Safety Related Acknowledgement",//13
    "Safety Related Broadcast Message",//14
    "Interrogation",//15
    "Assignment Mode Command",//16
    "DGNSS Broadcast Binary Message",//17
    "Standard Class B Equipment Position Report",//18
    "Extended Class B Equipment Position Report",//19
    "Data Link Management Message",//20
    "Aids-to-Navigation Report",//21
    "Channel management",//22
    "Group Assignment Command",//23
    "Static Data Report",//24
    "Single Slot Binary Message",//25
    "Multiple Slot Binary Message with Communications State",//26
    "Position Report for Long-Range Applications"//27
};
 
//The descriptions from Table 46 in ITU-R M.1371-5 
const char *AISReportDescriptions[numReportTypes] = {
    "",//There is no zero
    "Scheduled position report; (Class A shipborne mobile equipment)",//1
    "Assigned scheduled position report; (Class A shipborne mobile equipment)",//2
    "Special position report, response to interrogation; (Class A shipborne mobile equipment)",//3
    "Position, UTC, date and current slot number of base station",//4
    "Scheduled static and voyage related vessel data report; (Class A shipborne mobile equipment)",//5
    "Binary data for addressed communication",//6
    "Acknowledgement of received addressed binary data",//7
    "Binary data for broadcast communication",//8
    "Position report for airborne stations involved in search and rescue (SAR) operations, only",//9
    "Request UTC and date",//10
    "Current UTC and date if available",//11
    "Safety related data for addressed communication",//12
    "Acknowledgement of received addressed safety related message",//13
    "Safety related data for broadcast communication",//14
    "Request for a specific message type (can result in multiple responses from one or several stations)",//15
    "Assignment of a specific report behaviour by competent authority using a Base station",//16
    "DGNSS corrections provided by a base station",//17
    "Standard position report for Class B shipborne mobile equipment to be used instead of Messages 1, 2, 3",//18
    "No longer required; Extended position report for Class B shipborne mobile equipment; contains additional static information",//19
    "Reserve slots for Base station(s)",//20
    "Position and status report for aids-to-navigation",//21
    "Management of channels and transceiver modes by a Base station",//22
    "Assignment of a specific report behaviour by competent authority using a Base station to a specific group of mobiles",//23
    "Additional data assigned to an MMSI Part A: Name Part B: Static Data",//24
    "Short unscheduled binary data transmission (Broadcast or addressed)",//25
    "Scheduled binary data transmission (Broadcast or addressed)",//26
    "Class A and Class B 'SO' shipborne mobile equipment outside base station coverage"//27
};

enum AIS_STATUS_EXTENDED {
  AIS_BAD_CHECKSUM=AIS_STATUS_NUM_CODES,
  AIS_BAD_FRAGMENTS,
  AIS_STATUS_NUM_CODES_EXTENDED
};

//Prototypes of all of the functions in this file
AIS_STATUS decodeCoreMessage2Matlab(mxArray **matlabStruct, mxArray **reportName, mxArray **reportDescription, mxArray **fieldDescriptions, const char *theMessage, const int numPadBits);
bool NMEAChecksumValid(const char * const theMessage);
int combineMultipleNMEAMessages(const size_t curFrag, size_t *numReviewed, int *numPadBits, char **theMessage, const mxArray *cellArray);
AIS_STATUS AIS1_2_3ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS4_11ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS5ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS7_13ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS9ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS10ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS12ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS14ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS15ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS16ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS17ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS18ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS19ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS20ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS21ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS22ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS23ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS24ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS25ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS26ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS27ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_0_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_1_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_2_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_3_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_4_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_40_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS_FI_6_1_12_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_14_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_18_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_20_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_25_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS_FI_6_1_28_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS_FI_6_1_30_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_FI_6_1_32_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS6ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_0_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_11_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_13_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_15_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_16_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_17_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_19_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS_8_1_21_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_22_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_24_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_26_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_27_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_29_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_1_31_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS_8_1_40_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_200_10_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
//AIS_STATUS AIS_8_200_23_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_200_24_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_200_40_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS_8_200_55_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);
AIS_STATUS AIS8ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad);

using namespace std;
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) { 
    mxArray *decodedStruct,*reportName,*reportDescription,*fieldDescriptions;
    int retVal;//Hold enum returned value of type AIS_STATUS or AIS_STATUS_EXTENDED
    char *theMessage;
    int numPadBits=0;
    size_t numFragsUsed=1;
    bool isCellArray, decodeAll=false;
    //If a cell array is passed and decodeAll=true, then these will hold
    //the cell arrays for the return values.
    mxArray *decodedMessageCell, *numFragsUsedCell, *errorDescCell, *reportNameCell, *reportDescriptionCell, *fieldDescriptionsCell;
    
    if(nrhs!=1&&nrhs!=2) {
        mexErrMsgTxt("Wrong number of inputs.");
        return;
    }
    
    isCellArray=mxIsCell(prhs[0]);
    
    if(nrhs>1&&!isCellArray) {
        numPadBits=getIntFromMatlab(prhs[1]);
    } else if(nrhs>1&&isCellArray) {
        decodeAll=getBoolFromMatlab(prhs[1]);
    }
    
    if(nlhs>6) {
        mexErrMsgTxt("Wrong number of outputs.");
        return;
    }
    
    if(mxGetNumberOfDimensions(prhs[0])>2||mxGetNumberOfDimensions(prhs[0])<1) {
        mexErrMsgTxt("The input has an invalid dimensionality.");
        return;
    }

    //This function must be called before any AIS messages can be
    //decoded.
    BuildNmeaLookup();

    //The retVal is used as a flag here before calling the decoding routine
    //to detect whether an error occurred during the initial message
    //extraction step.

    //If it is a cell array, then assume we have multiple strings as NMEA
    //messages to paste together into a single data payload if their
    //checksums are valid. If it is a character string, then assume we
    //either have a payload or an NMEA message from which the payload
    //should be extracted if the checksum is valid.
    if(isCellArray){
        size_t curFrag=0;//Needed when decoding multiple messages in a cell.
        size_t numMsg=0;//Number of messages decoded
        const size_t numberOfFragments=max(mxGetM(prhs[0]),mxGetN(prhs[0]));
        mwSize cellArrayDims[2]={numberOfFragments,1};
        //Allocate the cell arrays for the return variables if multiple
        //messages are to be decoded. They will be allocated to the size of
        //numberOfFragments. However the number of messages might be less
        //than the number of fragments due to multifragment messages.
        //The outputs are set at the same time here.
        if(decodeAll) {
            decodedMessageCell=mxCreateCellArray(2, cellArrayDims);
            plhs[0]=decodedMessageCell;
            if(nlhs>1) {
                numFragsUsedCell=mxCreateCellArray(2, cellArrayDims);
                plhs[1]=numFragsUsedCell;
                if(nlhs>2) {
                    errorDescCell=mxCreateCellArray(2, cellArrayDims);
                    plhs[2]=errorDescCell;
                    if(nlhs>3) {
                        reportNameCell=mxCreateCellArray(2, cellArrayDims);
                        plhs[3]=reportNameCell;
                        if(nlhs>4){
                            reportDescriptionCell=mxCreateCellArray(2, cellArrayDims);
                            plhs[4]=reportDescriptionCell;
                            if(nlhs>5) {
                                fieldDescriptionsCell=mxCreateCellArray(2, cellArrayDims);
                                plhs[5]=fieldDescriptionsCell;
                            }
                        }
                    }
                }
            }
        }
        
        do {//Decode all or one of the messages 
            retVal=combineMultipleNMEAMessages(curFrag,&numFragsUsed,&numPadBits, &theMessage, prhs[0]);

            //Decode the message
            if(retVal==AIS_OK) {
                if(nlhs<2) {
                    retVal=decodeCoreMessage2Matlab(&decodedStruct, NULL, NULL, NULL, theMessage,numPadBits);
                } else if(nlhs<3) {
                    retVal=decodeCoreMessage2Matlab(&decodedStruct, &reportName, NULL, NULL, theMessage,numPadBits);
                } else if(nlhs<4) {
                    retVal=decodeCoreMessage2Matlab(&decodedStruct, &reportName, &reportDescription, NULL, theMessage,numPadBits);
                } else {
                    retVal=decodeCoreMessage2Matlab(&decodedStruct, &reportName, &reportDescription, &fieldDescriptions, theMessage,numPadBits);
                }
                //Free the space allocated for the string.
                mxFree(theMessage);//Free the buffer.
            }
            
            if(decodeAll) {
                //Set the outputs to return
                if(retVal==AIS_OK) {
                    mxSetCell(decodedMessageCell, numMsg, decodedStruct);
                    if(nlhs>1) {
                        mxSetCell(numFragsUsedCell, numMsg, unsignedSizeMat2Matlab(&numFragsUsed,1,1));
                        if(nlhs>2) {
                            mxSetCell(errorDescCell, numMsg, intMat2MatlabDoubles(&retVal,1,1));
                            if(nlhs>3) {
                                mxSetCell(reportNameCell, numMsg, reportName);
                                if(nlhs>4) {
                                    mxSetCell(reportDescriptionCell, numMsg, reportDescription);
                                    if(nlhs>5) {
                                        mxSetCell(fieldDescriptionsCell, numMsg, fieldDescriptions);
                                    }
                                }
                            }
                        }
                    }
                } else {
                    mxSetCell(decodedMessageCell, numMsg, mxCreateDoubleMatrix(0,0,mxREAL));       
                    if(nlhs>1) {
                        mxSetCell(numFragsUsedCell, numMsg, unsignedSizeMat2Matlab(&numFragsUsed,1,1));
                        if(nlhs>2) {
                            mxSetCell(errorDescCell, numMsg, intMat2MatlabDoubles(&retVal,1,1));
                            if(nlhs>3) {
                                mxSetCell(reportNameCell, numMsg, mxCreateDoubleMatrix(0,0,mxREAL));
                                if(nlhs>4) {
                                    mxSetCell(reportDescriptionCell, numMsg, mxCreateDoubleMatrix(0,0,mxREAL));
                                    if(nlhs>5) {
                                        mxSetCell(fieldDescriptionsCell, numMsg, mxCreateDoubleMatrix(0,0,mxREAL));
                                    }
                                }
                            }
                        }
                    }
                }
                //Increment the number of messages decoded.
                numMsg=numMsg+1;
            } else {
                break;
            }
            curFrag=curFrag+numFragsUsed;
        } while(curFrag<numberOfFragments);
        //If only 1 cell message is being decoded, then the same methods to
        //set the outputs can be used as for the case where a single string
        //is passed. For cells, the outputs were set when the cell arrays
        //were created. However, due to multifragment messages, there might
        //be more cells than decoded messages, so the outputs will be
        //reshaped here prior to returning.
        
        if(decodeAll) {
            cellArrayDims[0]=numMsg;

            mxSetDimensions(decodedMessageCell, cellArrayDims,2); 
            if(nlhs>1) {
                mxSetDimensions(numFragsUsedCell, cellArrayDims,2);
                if(nlhs>2) {
                    mxSetDimensions(errorDescCell, cellArrayDims,2);
                    if(nlhs>3) {
                    mxSetDimensions(reportNameCell, cellArrayDims,2);
                        if(nlhs>4) {
                            mxSetDimensions(reportDescriptionCell, cellArrayDims,2);
                            if(nlhs>5) {
                                mxSetDimensions(fieldDescriptionsCell, cellArrayDims,2);
                            }
                        }
                    }
                }
            }
            return;   
        }
    } else if(mxIsChar(prhs[0])) {
        retVal=AIS_OK;
        numFragsUsed=1;
    //The message must have at least enough characters for the header so
    //that we do not read past the end of an array while trying to identify
    //it.
        if(mxGetM(prhs[0])<7&&mxGetN(prhs[0])<7) {
            retVal=AIS_ERR_BAD_BIT_COUNT;
        } else {
            theMessage=mxArrayToString(prhs[0]);
            
            //If a full NMEA message was passed, validate the checksum,
            //find the number of padding bits and return the data payload.
            if(theMessage[0]=='!'||theMessage[0]=='$') {                
                //If the number of padding bits or the checksum is invalid.

                if(!NMEAChecksumValid(theMessage)){
                    mxFree(theMessage);
                    retVal=AIS_BAD_CHECKSUM;
                } else  if(numPadBits<0) {
                    mxFree(theMessage);
                    retVal=AIS_ERR_BAD_MSG_CONTENT;
                } else {
                 //If the checksum is valid, then return the message
                 //content.
                    string dataSubString=GetBody(string(theMessage));
                    mxFree(theMessage);
                    
                    //If the string returned is invalid
                    if(dataSubString.empty()) {
                        retVal=AIS_ERR_BAD_MSG_CONTENT;
                    } else {
                        //Copy the substring the theMessage. The string
                        //class itself will be deleted when leaving the
                        //scope where it is declared.
                        theMessage=(char*)mxMalloc((dataSubString.length()+1)*sizeof(char)); 
                        strcpy(theMessage, dataSubString.c_str());
                    }
                }
            }
        }
        
        if(retVal==AIS_OK) {
            if(nlhs<2) {
                retVal=decodeCoreMessage2Matlab(&decodedStruct, NULL, NULL, NULL, theMessage,numPadBits);
            } else if(nlhs<3) {
                retVal=decodeCoreMessage2Matlab(&decodedStruct, &reportName, NULL, NULL, theMessage,numPadBits);
            } else if(nlhs<4) {
                retVal=decodeCoreMessage2Matlab(&decodedStruct, &reportName, &reportDescription, NULL, theMessage,numPadBits);
            } else {
                retVal=decodeCoreMessage2Matlab(&decodedStruct, &reportName, &reportDescription, &fieldDescriptions, theMessage,numPadBits);
            }
            //Free the space allocated for the string.
            mxFree(theMessage);
        }
    } else {
        mexErrMsgTxt("The input must be a string of characters or a 1D cell array containing characters.");
        return; 
    }
    
    if(retVal==AIS_OK) {
        plhs[0]=decodedStruct;
        if(nlhs>1) {
            plhs[1]=unsignedSizeMat2Matlab(&numFragsUsed,1,1);
            if(nlhs>2) {
                plhs[2]=intMat2MatlabDoubles(&retVal,1,1);
                if(nlhs>3) {
                    plhs[3]=reportName;
                    if(nlhs>4) {
                        plhs[4]=reportDescription;
                        if(nlhs>5) {
                            plhs[5]=fieldDescriptions;
                        }
                    }
                }
            }
        }
    } else {
        plhs[0]=mxCreateDoubleMatrix(0,0,mxREAL);
        
        if(nlhs>1) {
            plhs[1]=unsignedSizeMat2Matlab(&numFragsUsed,1,1);
            if(nlhs>2) {
                plhs[2]=intMat2MatlabDoubles(&retVal,1,1);
                if(nlhs>3) {
                    plhs[3]=mxCreateDoubleMatrix(0,0,mxREAL);
                    if(nlhs>4) {
                        plhs[4]=mxCreateDoubleMatrix(0,0,mxREAL);
                        if(nlhs>5) {
                            plhs[5]=mxCreateDoubleMatrix(0,0,mxREAL);
                        }
                    }
                }
            }
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

int combineMultipleNMEAMessages(const size_t curFrag, size_t *numReviewed, int *numPadBits, char **theMessage, const mxArray *cellArray) {
//numPadBits will hold the number of padding bits on the last message that
//gets concatenated and theMessage will point to the concatenated message,
//if AIS_OK is the return value. numReviewed is the number of fragments
//that have either been put together to form a message, if
//AIS_STATUS==AIS_OK, or the number of fragments that have been considered
//and been found invalid.
//The curFrag parameter lets the decoding start/ continue from an arbitrary
//place in the cell array.
    const size_t numRows=mxGetM(cellArray);
    const size_t numCols=mxGetN(cellArray);
    const size_t numberOfMessages=max(numRows,numCols);
    ptrdiff_t totalNumFragments;
    size_t i;
    int sequentialMessageID;
    string combinedString;
    mxArray *curCell;
    char *curChars;

    //If a 2D cell array is passed, then it is unclear which order the
    //messages come in, so we will just return an error.
    if(mxIsEmpty(cellArray)||(numRows!=1&&numRows!=1)) {
        mexErrMsgTxt("A cell array that is passed must be 1-dimensional.");
    }
    
    //The number of cells reviewed.;
    *numReviewed=0;
    
    totalNumFragments=-1;
    for(i=curFrag;(i<numberOfMessages)&&(*numReviewed!=totalNumFragments);i++) {
        string theCellString;
        string tempString;
        
        *numReviewed=*numReviewed+1;
        
        //Get the ith cell entry
        curCell=mxGetCell(cellArray, i);
        if(!mxIsChar(curCell)||mxIsEmpty(curCell)) {
            mexErrMsgTxt("The cell array can only contain character strings.");
        }
        
        curChars=mxArrayToString(curCell);
        theCellString=string(curChars);
        
        //Getting the number of padding bits verifies that it has the
        //proper format up to the checksum and that the number given for
        //padding is not outside of the valid range. The padding number
        //should not be nonzero for any of the fragments before the last
        //one.
        *numPadBits=GetPad(theCellString);
        if(!NMEAChecksumValid(curChars)){
            mxFree(curChars);
            return AIS_BAD_CHECKSUM;   
        } else  if(*numPadBits<0) {
            mxFree(curChars);
            return AIS_ERR_BAD_MSG_CONTENT;
        }
        
        //Check that it has the proper sequence numbering.
        {
            size_t firstNum, secondNum;
            tempString=GetNthField(theCellString,1,string(","));
            //If an error occurred extracting the first message number 
            if(tempString.empty()) {
                return AIS_ERR_BAD_MSG_CONTENT;
            }
            
            firstNum=strtoul(tempString.c_str(),NULL,10);
            //0 is not a valid value and is returned if strtoul fails.
            if(firstNum==0) {
                return AIS_ERR_BAD_MSG_CONTENT;
            }
                    
            tempString=GetNthField(theCellString,2,string(","));
            //If an error occurred extracting the second message number 
            if(tempString.empty()) {
                return AIS_ERR_BAD_MSG_CONTENT;
            }
            
            secondNum=strtoul(tempString.c_str(),NULL,10);
            //0 is not a valid value and is returned if strtoul fails.
            if(secondNum==0) {
                return AIS_ERR_BAD_MSG_CONTENT;
            }

            //Verify the sequencing numbers. If this is the first fragment
            //taken, record the total number of fragments.
            if(*numReviewed==1) {
                totalNumFragments=firstNum;
                //There are not enough fragments to decode the message.
                if(totalNumFragments>(ptrdiff_t)(numberOfMessages-curFrag)) {
                    return AIS_BAD_FRAGMENTS;
                }
            }
            
            if(firstNum!=totalNumFragments||secondNum!=*numReviewed) {
            //If this is the case, then message i is a different fragment
            //than the one before and unto itself might be a valid message,
            //so we cannot mark it as having been reviewed to be valid.
                *numReviewed=*numReviewed-1;
                return AIS_BAD_FRAGMENTS;
            }
            
            //The sequential message ID has to be the same for all of the
            //messages that come from the same sequence. It varies from 0
            //to 9.
            tempString=GetNthField(theCellString,1,string(","));
            if(*numReviewed==1) {
                //By setting it to -1, we are choosing a value that will
                //not come up again.
                if(tempString.empty()) {
                    sequentialMessageID=-1;
                } else if(tempString.length()==1) {
                    int diff=(tempString[0]-'0');
                    
                    if((diff<0)||(diff>9)) {
                        return AIS_ERR_BAD_MSG_CONTENT;
                    }
                    sequentialMessageID=diff;
                } else {
                    return AIS_ERR_BAD_MSG_CONTENT;
                }
            } else {
                int diff=tempString[0]-'0';
                if(tempString[0]!='\0'&&((diff<0)||(diff>9))) {
                    return AIS_ERR_BAD_MSG_CONTENT;
                } else if(diff!=sequentialMessageID){
                //The current message belongs to a different sequence and
                //should not be marked as having been reviewed to be
                //invalid.
                    *numReviewed=*numReviewed-1;
                    return AIS_BAD_FRAGMENTS;
                }
            }
        }
               
        tempString=GetBody(theCellString);
        mxFree(curChars);
        
        //If an error occurred extracting the characters
        if(tempString.empty()) {
            return AIS_ERR_BAD_MSG_CONTENT;
        }

        //Otherwise, append it to the combined string.
        combinedString.append(tempString);
    }
    
    //Set the string to return.
    *theMessage=(char*)mxMalloc((combinedString.length()+1)*sizeof(char)); 
    strcpy(*theMessage, combinedString.c_str());
    return AIS_OK;
}


AIS_STATUS decodeCoreMessage2Matlab(mxArray **matlabStruct, mxArray **reportName, mxArray **reportDescription, mxArray **fieldDescriptions, const char *theMessage, const int numPadBits) {
//The BuildNmeaLookup function must be called before this function can be
//used.
    //This is space for the message header after being turned into a bunch
    //of bits. It is 7 characters at 6 bits each.
    bitset<42> bs;
    int message_id,repeat_indicator,mmsi;//For the message header terms
    const size_t msgLength=strlen(theMessage);
    if(msgLength<7) {//There are not enough characters in the message for the header.
        return AIS_ERR_BAD_BIT_COUNT;
    }
    const string messageHeader(theMessage, 7);
    AIS_STATUS r; 
    AIS_STATUS retVal;
    const size_t numBits = msgLength*6-numPadBits;
    if (numBits < 38) {//Make sure that enough bits for the header are present.
        return AIS_ERR_BAD_BIT_COUNT;
    }
    
    r=aivdm_to_bits(bs, messageHeader.c_str());
    if(r!=AIS_OK) {
        return r;
    }
  
    message_id = ubits(bs, 0, 6);
    repeat_indicator = ubits(bs, 6, 2);
    mmsi = ubits(bs, 8, 30);
    switch(message_id) {
        case 1:
        case 2:
        case 3:
            retVal=AIS1_2_3ToMatlab(matlabStruct,fieldDescriptions, theMessage, numPadBits);
            break;
        case 4:
        case 11:
            retVal=AIS4_11ToMatlab(matlabStruct,fieldDescriptions, theMessage, numPadBits);
            break;
        case 5:
            retVal=AIS5ToMatlab(matlabStruct,fieldDescriptions, theMessage, numPadBits);
            break;
        case 6:
        {//The type of the submessage must be identified.
            Ais6 msg(theMessage, numPadBits);
            if (msg.had_error()&&msg.get_error()!=AIS_UNINITIALIZED) {
                retVal=msg.get_error();
                break;
            }
            switch(msg.dac) {
                case AIS_DAC_1_INTERNATIONAL://IMO
                    switch(msg.fi) {
                        case AIS_FI_6_1_0_TEXT:
                            retVal=AIS_FI_6_1_0_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_1_ACK:
                            retVal=AIS_FI_6_1_1_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_2_FI_INTERROGATE:
                            retVal=AIS_FI_6_1_2_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_3_CAPABILITY_INTERROGATE:
                            retVal=AIS_FI_6_1_3_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_4_CAPABILITY_REPLY:
                            retVal=AIS_FI_6_1_4_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
//                         case AIS_FI_6_1_12_DANGEROUS_CARGO:
//                             retVal=AIS_FI_6_1_12_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//                             break;
                        case AIS_FI_6_1_14_TIDAL_WINDOW:
                            retVal=AIS_FI_6_1_14_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_18_ENTRY_TIME:
                            retVal=AIS_FI_6_1_18_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_20_BERTHING:
                            retVal=AIS_FI_6_1_20_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_25_DANGEROUS_CARGO:
                            retVal=AIS_FI_6_1_25_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
//                         case AIS_FI_6_1_28_ROUTE:
//                             retVal=AIS_FI_6_1_28_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//                             break;
//                         case AIS_FI_6_1_30_TEXT:
//                             retVal=AIS_FI_6_1_30_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//                             break;
                        case AIS_FI_6_1_32_TIDAL_WINDOW:
                            retVal=AIS_FI_6_1_32_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_6_1_40_PERSONS_ON_BOARD:
                            retVal=AIS_FI_6_1_40_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        default:
                        //If it is not a known type, return the parts that are
                        //known common to all AIS6 messages.
                            retVal=AIS6ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                    }
                    break;
                default:
                    //If it is not a known type, return the parts that are
                    //known common to all AIS6 messages.
                    retVal=AIS6ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                    break;
            }
            break;
        }
        case 7:
        case 13:
            retVal=AIS7_13ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 8:
        {
            Ais8 msg(theMessage, numPadBits);
            
            if (msg.had_error()&&msg.get_error()!=AIS_UNINITIALIZED) {
                retVal=msg.get_error();
                break;
            }
            
            switch(msg.dac) {
                case AIS_DAC_1_INTERNATIONAL://IMO
                    switch (msg.fi){
                        case AIS_FI_8_1_0_TEXT:
                            retVal=AIS_8_1_0_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_11_MET_HYDRO:
                            retVal=AIS_8_1_11_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_13_FAIRWAY_CLOSED:
                            retVal=AIS_8_1_13_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_15_SHIP_AND_VOYAGE:
                            retVal=AIS_8_1_15_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_16_PERSONS_ON_BOARD:
                            retVal=AIS_8_1_16_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_17_VTS_TARGET:
                            retVal=AIS_8_1_17_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_19_TRAFFIC_SIGNAL:
                            retVal=AIS_8_1_19_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
//                         case AIS_FI_8_1_21_WEATHER_OBS:
//                             retVal=AIS_8_1_21_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//                             break;
                        case AIS_FI_8_1_22_AREA_NOTICE:
                            retVal=AIS_8_1_22_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_24_SHIP_AND_VOYAGE:
                            retVal=AIS_8_1_24_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_26_SENSOR:
                            retVal=AIS_8_1_26_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_27_ROUTE:
                            retVal=AIS_8_1_27_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_29_TEXT:
                            retVal=AIS_8_1_29_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_1_31_MET_HYDRO:
                            retVal=AIS_8_1_31_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
//                         case AIS_FI_8_1_40_PERSONS_ON_BOARD:
//                             retVal=AIS_8_1_40_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//                             break;
                        default:
                            //If it is not a known type, return the parts that are
                            //known common to all AIS8 messages.
                            retVal=AIS8ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                    }
                    break;
                case AIS_DAC_200_RIS:
                    switch(msg.fi) {
                        case AIS_FI_8_200_10_RIS_SHIP_AND_VOYAGE:
                            retVal=AIS_8_200_10_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
//                         case AIS_FI_8_200_23_RIS_EMMA_WARNING:
//                             retVal=AIS_8_200_23_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//                             break;
                        case AIS_FI_8_200_24_RIS_WATERLEVEL:
                            retVal=AIS_8_200_24_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_200_40_RIS_ATON_SIGNAL_STATUS:
                            retVal=AIS_8_200_40_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        case AIS_FI_8_200_55_RIS_PERSONS_ON_BOARD:
                            retVal=AIS_8_200_55_ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                        default:
                            //If it is not a known type, return the parts that are
                            //known common to all AIS8 messages.
                            retVal=AIS8ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                            break;
                    }
                    break;
                default:
                    //If it is not a known type, return the parts that are
                    //known common to all AIS8 messages.
                    retVal=AIS8ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
                    break;
            }
            break;
        }
        break;
        case 9:
            retVal=AIS9ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 10:
            retVal=AIS10ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        //Case 11 is taken care of with case 4
        case 12:
            retVal=AIS12ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        //Case 13 is taken care of with case 7
        case 14:
            retVal=AIS14ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
//         case 15:
//             retVal=AIS15ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
//             break;
        case 16:
            retVal=AIS16ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 17:
            retVal=AIS17ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 18:
            retVal=AIS18ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 19:
            retVal=AIS19ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 20:
            retVal=AIS20ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 21:
            retVal=AIS21ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 22:
            retVal=AIS22ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 23:
            retVal=AIS23ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 24:
            retVal=AIS24ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 25:
            retVal=AIS25ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 26:
            retVal=AIS26ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        case 27:
            retVal=AIS27ToMatlab(matlabStruct, fieldDescriptions, theMessage, numPadBits);
            break;
        default://Unknown AIS Message. Just return the components common to
        //all AIS messages
        {
            const size_t numberOfFields=3;
            const mwSize dims[2] = {1, 1};
            const char *fieldNames[numberOfFields] = {"message_id",
            "repeat_indicator","mmsi"};
            
            //Create the Matlab structure array.
            *matlabStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    
            //Fill all of the elements of the structure
            mxSetFieldByNumber(*matlabStruct,0,0,intMat2MatlabDoubles(&message_id,1,1));
            mxSetFieldByNumber(*matlabStruct,0,1,intMat2MatlabDoubles(&repeat_indicator,1,1));
            mxSetFieldByNumber(*matlabStruct,0,2,intMat2MatlabDoubles(&mmsi,1,1));

            retVal=AIS_ERR_UNKNOWN_MSG_TYPE;
        }
    }
    
    if(retVal==AIS_OK||retVal==AIS_ERR_UNKNOWN_MSG_TYPE) {
        if(reportName!=NULL) {
            if(retVal!=AIS_ERR_UNKNOWN_MSG_TYPE) {
                const char *charString=AISReportNames[message_id];
                *reportName=mxCreateCharMatrixFromStrings(1,&charString);
            } else {
                const char *charString="Unknown Report Type";
                *reportName=mxCreateCharMatrixFromStrings(1,&charString);
            }
        }
        if(reportDescription!=NULL) {
            if(retVal!=AIS_ERR_UNKNOWN_MSG_TYPE) {
                const char *charString=AISReportDescriptions[message_id];
                *reportDescription=mxCreateCharMatrixFromStrings(1,&charString);
            } else {
                const char *charString="Unknown Report Type";
                *reportDescription=mxCreateCharMatrixFromStrings(1,&charString);
            }
        }
    }
    
    //It is okay in that it decoded the enough fields to determine the
    //message ID.
    if(retVal==AIS_ERR_UNKNOWN_MSG_TYPE) {
        retVal=AIS_OK;
    }
    
    return retVal;
}

AIS_STATUS AIS1_2_3ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=26;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "nav_status","rot", "sog",
    "position_accuracy", "x", "y", "cog", "true_heading", "timestamp",
    "special_manoeuvre", "spare", "raim", "sync_state", "slot_timeout",
    "received_stations", "slot_number", "utc_hour", "utc_min", "utc_spare",
    "slot_offset", "slot_increment", "slots_to_allocate", "keep_flag"};
    mxArray *theStruct;
    
    Ais1_2_3 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
        "Message identifier.",//0
        "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
        "Unique identifier such as a Maritime mobile service identity (MMSI) number",//2
        "0=under way using engine\n1=at anchor,\n2=not under command\n3=restricted maneuverability\n4=constrained by her draught\n5=moored\n6=aground\n7=engaged in fishing\n8=under way sailing\n9=reserved for future amendment of navigational status for ships carrying DG, HS, or MP, or IMO hazard or pollutant category C, high speed craft (HSC)\n10=reserved for future amendment of navigational status for ships carrying dangerous goods (DG), harmful substances (HS) or marine pollutants (MP), or IMO hazard or pollutant category A, wing in ground (WIG)\n11=power-driven vessel towing astern (regional use)\n12=power-driven vessel pushing ahead or towing alongside (regional use)\n13 = reserved for future use\n14=AIS-SART (active), MOB-AIS, EPIRB-AIS\n15=undefined = default (also used by AIS-SART, MOB-AIS and EPIRB- AIS under test)",//3
         "Rate of turn in degrees per minute clockwise",//4
         "Speed over ground in knots. 10.2. means 102.2 knots or higher",//5
         "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//6
         "Longitude East in degrees (WGS-84)",//7
         "Latitude North in degrees (WGS-84)",//8
         "Course over ground in degrees East of North",//9
         "True heading (the direction the ship is pointing/ would be going without wind/currents) in degrees East of North",//10
         "UTC second when report was generated or 61 if positioning system is in manual input mode or 62 if in dead reckining mode or 63 if positioning system is inoperative",//11
         "1=not engaged in special manoeuvre\n2=engaged in special manoeuvre\n(i.e. regional passing arrangement on Inland Waterway)",//12
         "Not used. Should be set to zero. Reserved for future use.",//13
         "Receiver autonomous integrity monitoring (RAIM) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use.",//14
         "Synchronization state\n0 UTC direct\n1 UTC indirect\n2 Station is synchronized to a base station (base direct)\n3 Station is synchronized to another station based on the highest number of received stations or to another mobile station, which is directly synchronized to a base station",//15
         "(For Message IDs 1 and 2) Specifies frames remaining until a new slot is selected\n0 means that this was the last transmission in this slot\n1-7 means that 1 to 7 frames respectively are left until slot change",//16
         "(For Message IDs 1 and 2) Number of other stations (not own station) which the station currently is receiving (between 0 and 16 383).",//17
         "(For Message IDs 1 and 2) Slot number used for this transmission (between 0 and 2 249).",//18
         "(For Message IDs 1 and 2) Hour (0-23) in UTC time, if available",//19
         "(For Message IDs 1 and 2) Minute (0-23) in UTC time, if available",//20
         "(For Message IDs 1 and 2) Two extra (unused) bits that are associated with the time message",//21
         "(For Message IDs 1 and 2) If the slot time-out value is 0 (zero) then the slot offset should indicate the offset to the slot in which transmission will occur during the next frame. If the slot offset is zero, the slot should be de-allocated after transmission.",//22
         "(For Message ID 3) Offset to next slot to be used, or zero (0) if no more transmissions",//23
         "(For Message ID 3) Number of consecutive slots to allocate.\n0 = 1 slot,\n1 = 2 slots,\n2 = 3 slots,\n3 = 4 slots,\n4 = 5 slots,\n5 = 1 slot; offset = slot increment + 8 192,\n6 = 2 slots; offset = slot increment + 8 192,\n7 = 3 slots; offset = slot increment + 8 192.\nUse of 5 to 7 removes the need for RATDMA broadcast for scheduled transmissions up to 6 min intervals",//24
          "(For Message ID 3) Set to TRUE = 1 if the slot remains allocated for one additional frame"//25
        };
        int i;
        
        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
        
        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.nav_status,1,1));
    
    if(!msg.rot_over_range) {
       mxSetFieldByNumber(theStruct,0,4,floatMat2MatlabDoubles(&msg.rot,1,1));
    } else {//If the rotation rate is invalid.
       mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.sog<=102.2) {
        mxSetFieldByNumber(theStruct,0,5,floatMat2MatlabDoubles(&msg.sog,1,1));
    } else {//Speed over ground is not available.
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//If longitude is not available
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.cog<360) {
        mxSetFieldByNumber(theStruct,0,9,floatMat2MatlabDoubles(&msg.cog,1,1));
    } else {//Course over ground is not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.true_heading<360) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.true_heading,1,1));
    } else {//True Heading is not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.timestamp!=60) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.timestamp,1,1));  
    } else {//If no time stamp is available.
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.special_manoeuvre!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.special_manoeuvre,1,1)); 
    } else {//Special maneuver information is not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.spare,1,1)); 
    mxSetFieldByNumber(theStruct,0,14,boolMat2Matlab(&msg.raim,1,1));

    //COMM States
    mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.sync_state,1,1)); 
    //SODATA
    if(msg.message_id==1||msg.message_id==2) {
        if(msg.slot_timeout_valid) {
            mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.slot_timeout,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.received_stations_valid) {
            mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.received_stations,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.slot_number_valid) {
            mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.slot_number,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.utc_valid) {
            mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.utc_hour,1,1));
            mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.utc_min,1,1));
            mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.utc_spare,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
            mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
            mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.slot_offset_valid) {
            mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.slot_offset,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    } else {
        //For a type 3 message, these fields are not present, so they will
        //be empty matrices.
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    //ITDMA
    if(msg.slot_increment_valid) {
        mxSetFieldByNumber(theStruct,0,23,intMat2MatlabDoubles(&msg.slot_increment,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slots_to_allocate_valid) {
        mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.slots_to_allocate,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.keep_flag_valid) {
        mxSetFieldByNumber(theStruct,0,25,boolMat2Matlab(&msg.keep_flag,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    return AIS_OK;
}

AIS_STATUS AIS4_11ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=24;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "year", "month", "day", "hour", "minute", "second",
    "position_accuracy", "x", "y", "fix_type", "transmission_ctl", "spare",
    "raim", "sync_state", "slot_timeout", "received_stations",
    "slot_number", "utc_hour", "utc_min","utc_spare", "slot_offset"};
    mxArray *theStruct;
    
    Ais4_11 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    //If text descrptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for this message\n4 = UTC and position report from base station\n11 = UTC and position response from mobile station",//0
            "Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more",//1
            "Maritime mobile service identity (MMSI) number",//2
            "UTC year (1-9999)",//3
            "UTC month (1-12)",//4
            "UTC day (1-31)",//5
            "UTC hour (0-23)",//6
            "UTC minute (0-59)",//7
            "UTC second (0-59)",//8
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//9
            "Longitude East in degrees (WGS-84)",//10
            "Latitude North in degrees (WGS-84)",//11
            "Type of electronic position fixing device. Use of differential corrections is defined by field position accuracy above:\n1 = global positioning system (GPS)\n2 = GNSS (GLONASS)\n3 = combined GPS/GLONASS\n4 = Loran-C\n5 = Chayka\n6 = integrated navigation system\n7 = surveyed\n8 = Galileo\n9-14 = not used\n15 = internal GNSS",//12
            "Transmission control for long- range broadcast message\n0 = default Class-A AIS station stops transmission of Message 27 within an AIS base station coverage area.\n1 = Request Class-A station to transmit Message 27 within an AIS base station coverage area.",//13
            "Not used. Should be set to zero. Reserved for future use",//14
            "Receiver autonomous integrity monitoring (RAIM) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use.",//15
            "Synchronization state\n0 UTC direct\n1 UTC indirect\n2 Station is synchronized to a base station (base direct)\n3 Station is synchronized to another station based on the highest number of received stations or to another mobile station, which is directly synchronized to a base station",//16
            "(For Message IDs 1 and 2) Specifies frames remaining until a new slot is selected\n0 means that this was the last transmission in this slot\n1-7 means that 1 to 7 frames respectively are left until slot change",//17
            "Number of other stations (not own station) which the station currently is receiving (between 0 and 16 383).",//18
            "Slot number used for this transmission (between 0 and 2 249).",//19
            "Hour (0-23) in UTC time, if available",//20
            "Minute (0-23) in UTC time, if available",//21
            "Two extra (unused) bits that are associated with the time message",//22
            "If the slot time-out value is 0 (zero) then the slot offset should indicate the offset to the slot in which transmission will occur during the next frame. If the slot offset is zero, the slot should be de-allocated after transmission.",//23
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    if(msg.year!=0) {
        mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.year,1,1));
    } else {//UTC year not available
        mxSetFieldByNumber(theStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.month!=0) {
        mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.month,1,1));
    } else {//UTC month not available
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.day!=0) {
        mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.day,1,1));
    } else {//UTC day not available
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.hour<24) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.hour,1,1));
    } else {//UTC hour invalid or not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.minute<60) {
        mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.minute,1,1));
    } else {//UTC minute invalid or not available
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.second<60) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.second,1,1));
    } else {//UTC second invalid or not available
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,10,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//If longitude is not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,11,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//Latitude is not available
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.fix_type!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.fix_type,1,1));
    } else {//Fix type is unavailable
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.transmission_ctl,1,1));
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,15,boolMat2Matlab(&msg.raim,1,1));
    
    //SOTDMA
    mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.sync_state,1,1));
    mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.slot_timeout,1,1));

    if(msg.received_stations_valid) {
        mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.received_stations,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_number_valid) {
        mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.slot_number,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_valid) {
        mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.utc_hour,1,1));
        mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.utc_min,1,1));
        mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.utc_spare,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL)); 
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.slot_offset_valid) {
        mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.slot_offset,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    return AIS_OK;
}

AIS_STATUS AIS5ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=21;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "ais_version", "imo_num", "callsign", "name", "type_and_cargo",
    "dim_a", "dim_b", "dim_c", "dim_d", "fix_type", "eta_month", "eta_day",
    "eta_hour", "eta_minute", "draught", "destination", "dte", "spare"};
    mxArray *theStruct;
    
    Ais5 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
             "Identifier for this message\n4 = UTC and position report from base station\n11 = UTC and position response from mobile station",//0
            "Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more",//1
            "Maritime mobile service identity (MMSI) number",//2
            "AIS version indicator\n0 = station compliant with Recommendation ITU-R M.1371-1\n1 = station compliant with Recommendation ITU-R M.1371-3 (or later)\n2 = station compliant with Recommendation ITU-R M.1371-5 (or later)\n3 = station compliant with future editions",//3
            "IMO Number\n0000000001-0000999999 not used\n0001000000-0009999999 = valid IMO number;\n0010000000-1073741823 = official flag state number.",//4
            "Call sign. Craft associated with a parent vessel, should use 'A' followed by the last 6 digits of the MMSI of the parent vessel. Examples of these craft include towed vessels, rescue boats, tenders, lifeboats and liferafts.",//5
            "Name. Maximum 20 characters. The Name should be as shown on the station radio license. For search and rescue (SAR) aircraft, it should be set to 'SAR AIRCRAFT NNNNNNN' where NNNNNNN equals the aircraft registration number.",//6
            "Type of ship and cargo type\n1-99 = as defined in Section 3.3.2 of Annex 8 of ITU-R M.1371-5\n100-199 = reserved, for regional use 200-255 = reserved, for future use\nNot applicable to search and rescue (SAR) aircraft",//7
            "Distance from reference point to bow of ship (meters) 511m means 511m or longer. (If only relative reference point position known=0)",//8
            "Distance from reference point to aft of ship (meters) 511m means 511m or longer. ",//9
            "Distance from reference point to port side of ship (meters) 511m means 511m or longer. (If only relative reference point position known=0)",//10
            "Distance from reference point to starboard side of ship (meters) 511m means 511m or longer.",//11
            "Type of electronic position fixing device:\n1 = global positioning system (GPS)\n2 = GNSS (GLONASS)\n3 = combined GPS/GLONASS\n4 = Loran-C\n5 = Chayka\n6 = integrated navigation system\n7 = surveyed\n8 = Galileo\n9-14 = not used\n15 = internal GNSS",//12
            "Estimated UTC month of arrival (1-12)",//13
            "Estimated UTC day of arrival (1-31)",//14
            "Estimated UTC hour of arrival (0-23)",//15
            "Estimated UTC minute of arrival (0-59)",//16
            "Maximum present static draught in meters. 255m means 255 meters or greater. Not applicable to search and rescue (SAR) aircraft",//17
            "Destination, maximum 20 characters. For search and rescue (SAR) aircraft, the use of this field may be decided by the responsible administration",//18
            "Data terminal equipment (DTE) ready (0 = available, 1 = not available = default)",//19
            "Spare. Not used. Should be set to zero. Reserved for future use"//20
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.ais_version,1,1));
    
    if(msg.imo_num!=0) {
        mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.imo_num,1,1));
    } else {//IMO number not available
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.callsign.compare("@@@@@@@")!=0) {
        const char *charString=msg.callsign.c_str();
        mxSetFieldByNumber(theStruct,0,5,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//If the call sign is not available
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.name.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.name.c_str();
        mxSetFieldByNumber(theStruct,0,6,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//If the name is not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.type_and_cargo!=0) {
        mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.type_and_cargo,1,1));
    } else {//No type and cargo information
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.dim_a==0&&msg.dim_b==0&&msg.dim_c==0&&msg.dim_d==0) {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    } else {//If ship dimensions from reference point are available.
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.dim_a,1,1));
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.dim_b,1,1));
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.dim_c,1,1));
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.dim_d,1,1));
    }
    
    if(msg.fix_type!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.fix_type,1,1));
    } else {//If the fix type is not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.eta_month!=0) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.eta_month,1,1));
    } else {//No eta month
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.eta_day!=0) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.eta_day,1,1));
    } else {//No eta day
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.eta_hour<24) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.eta_hour,1,1));
    } else {//No eta hour
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.eta_minute<60) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.eta_minute,1,1));
    } else {//No eta minute
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.draught!=0) {
        mxSetFieldByNumber(theStruct,0,17,floatMat2MatlabDoubles(&msg.draught,1,1));
    } else {//No draught available
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.destination.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.destination.c_str();
        mxSetFieldByNumber(theStruct,0,18,mxCreateCharMatrixFromStrings(1,&charString));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.dte,1,1));
    mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.spare,1,1));
    
    return AIS_OK;
}

AIS_STATUS AIS7_13ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=6;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "dest_mmsi", "seq_num", "spare"};
    mxArray *theStruct;
    
    Ais7_13 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Messages 7 or 13\n7 = binary acknowledge\n13 = safety related acknowledge",//0
            "Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more",//1
            "Maritime mobile service identity (MMSI) number of source of this acknowledge (ACK)",//2
            "An array of the destination MMSIs of the destinations of this ACK",//3
            "An array of the sequence numbers of the messages to be acknowledged for each destination (0-3)",//4
            "Spare bits. Not used. Should be set to zero. Reserved for future use"//5
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    {
        int *theData;
        theData=msg.dest_mmsi.data();
        mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(theData,msg.dest_mmsi.size(),1));
        theData=msg.seq_num.data();
        mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(theData,msg.seq_num.size(),1));
    }
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.spare,1,1)); 
    
    return AIS_OK;
}

AIS_STATUS AIS9ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=28;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "alt", "sog", "position_accuracy", "x", "y", "cog",
    "timestamp", "alt_sensor", "spare", "dte", "spare2", "assigned_mode",
    "raim", "commstate_flag", "sync_state", "slot_timeout",
    "received_stations", "slot_number", "utc_hour", "utc_min", "utc_spare",
    "slot_offset", "slot_increment", "slots_to_allocate", "keep_flag"};
    mxArray *theStruct;
    
    Ais9 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
        	"Message identifier.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number",//2
            "Altitude (derived from GNSS or barometric (see altitude sensor parameter below)) (m) (0-4 094 m) 4094 = 4094 m or higher",//3
            "Speed over ground in knots",//4
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//5
            "Longitude East in degrees (WGS-84)",//6
            "Latitude North in degrees (WGS-84)",//7
            "Course over ground in degrees East of North",//8
            "UTC second when report was generated or 61 if positioning system is in manual input mode or 62 if in dead reckining mode or 63 if positioning system is inoperative",//9
            "Altitude sensor. 0 = GNSS, 1 = barometric source",//10
            "Not used. Should be set to zero. Reserved for future use",//11
            "Data terminal ready (0 = available 1 = not available = default)",//12
            "Not used. Should be set to zero. Reserved for future use",//13
            "0 = Station operating in autonomous and continuous mode = default 1 = Station operating in assigned mode",//14
            "Receiver autonomous integrity monitoring (RAIM) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use.",//15
            "0 = SOTDMA communication state follows 1 = ITDMA communication state follows",//16
            "Synchronization state\n0 UTC direct\n1 UTC indirect\n2 Station is synchronized to a base station (base direct)\n3 Station is synchronized to another station based on the highest number of received stations or to another mobile station, which is directly synchronized to a base station",//17
            "(For SOTDMA) Specifies frames remaining until a new slot is selected\n0 means that this was the last transmission in this slot\n1-7 means that 1 to 7 frames respectively are left until slot change",//18
            "(For SOTDMA) Number of other stations (not own station) which the station currently is receiving (between 0 and 16 383).",//19
            "(For SOTDMA) Slot number used for this transmission (between 0 and 2 249).",//20
            "(For SOTDMA) Hour (0-23) in UTC time, if available",//21
            "(For SOTDMA) Minute (0-23) in UTC time, if available",//22
            "(For SOTDMA) Two extra (unused) bits that are associated with the time message",//23
            "(For SOTDMA) If the slot time-out value is 0 (zero) then the slot offset should indicate the offset to the slot in which transmission will occur during the next frame. If the slot offset is zero, the slot should be de-allocated after transmission.",//24
            "(For ITDMA) Offset to next slot to be used, or zero (0) if no more transmissions",//25
            "(For ITDMA) Number of consecutive slots to allocate.\n0 = 1 slot,\n1 = 2 slots,\n2 = 3 slots,\n3 = 4 slots,\n4 = 5 slots,\n5 = 1 slot; offset = slot increment + 8 192,\n6 = 2 slots; offset = slot increment + 8 192,\n7 = 3 slots; offset = slot increment + 8 192.\nUse of 5 to 7 removes the need for RATDMA broadcast for scheduled transmissions up to 6 min intervals",//26
            "(For ITDMA) Set to TRUE = 1 if the slot remains allocated for one additional frame"//27
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    
    if(msg.alt!=4095) {
        mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.alt,1,1)); 
    } else {//No altitude information available.
        mxSetFieldByNumber(theStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.sog!=1023) {
        mxSetFieldByNumber(theStruct,0,4,floatMat2MatlabDoubles(&msg.sog,1,1));
    } else {//No speed over ground information available
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//If the longitude is invalid
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {// If the latitude is invalid
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.cog<360) {
        mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.cog,1,1));
    } else {//If course over ground is invalid
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.timestamp!=60) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.timestamp,1,1)); 
    } else {//If the timestamp is invalid
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.alt_sensor,1,1)); 
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.spare,1,1)); 
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.dte,1,1)); 
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.spare2,1,1)); 
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.assigned_mode,1,1)); 
    mxSetFieldByNumber(theStruct,0,15,boolMat2Matlab(&msg.raim,1,1));
    mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.commstate_flag,1,1));    
    mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.sync_state,1,1));
    
    if(msg.slot_timeout_valid) {
        mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.slot_timeout,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.received_stations_valid) {
        mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.received_stations,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_number_valid) {
        mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.slot_number,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_valid) {
        mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.utc_hour,1,1));
        mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.utc_min,1,1));
        mxSetFieldByNumber(theStruct,0,23,intMat2MatlabDoubles(&msg.utc_spare,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.slot_offset_valid) {
        mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.slot_offset,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_increment_valid) {
        mxSetFieldByNumber(theStruct,0,25,intMat2MatlabDoubles(&msg.slot_increment,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slots_to_allocate_valid) {
        mxSetFieldByNumber(theStruct,0,26,intMat2MatlabDoubles(&msg.slots_to_allocate,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,26,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.keep_flag_valid) {
        mxSetFieldByNumber(theStruct,0,27,boolMat2Matlab(&msg.keep_flag,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,27,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    return AIS_OK;
}

AIS_STATUS AIS10ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=6;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dest_mmsi", "spare2"};
    mxArray *theStruct;
    
    Ais10 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }

    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 10; always 10.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of station which inquires UTC",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "MMSI number of station which is inquired",//4
            "Not used. Should be set to zero. Reserved for future use"//5
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
        
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dest_mmsi,1,1)); 
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.spare2,1,1)); 

    return AIS_OK;
}

AIS_STATUS AIS12ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=8;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq_num", "dest_mmsi", "retransmitted", "spare", "text"};
    mxArray *theStruct;
    
    Ais12 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 12; always 12.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of station which is the source of the message.",//2
            "Sequence number (0-3) as described in Section 5.3.1, Annex 2 of ITU-R M.1371-5",//3
            "MMSI number of station which is the destination of the message",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Safety related text."//7
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq_num,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dest_mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmitted,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    {
        const char *charString=msg.text.c_str();
        mxSetFieldByNumber(theStruct,0,7,mxCreateCharMatrixFromStrings(1,&charString));
    }

    return AIS_OK;
}

AIS_STATUS AIS14ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=5;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "text"};
    mxArray *theStruct;
    
    Ais14 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 14; always 14.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station of message.",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "Safety related text."//4
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    {
        const char *charString=msg.text.c_str();
        mxSetFieldByNumber(theStruct,0,4,mxCreateCharMatrixFromStrings(1,&charString));
    }

    return AIS_OK;
}

/*
AIS_STATUS AIS15ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=15;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "mmsi_1", "msg_1_1", "slot_offset_1_1", "spare2",
    "dest_msg_1_2","slot_offset_1_2","spare3","mmsi_2","msg_2",
    "slot_offset_2","spare4"};
    mxArray *theStruct;
    
    Ais15 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 15; always 15.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of interrogating station",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "MMSI number of first interrogated station",//4
            "First requested message type from first interrogated station",//5
            "Response slot offset for first requested message from first interrogated station",//6
            "Not used. Should be set to zero. Reserved for future use",//7
            "Second requested message type from first interrogated station",//8
            "Response slot offset for second requested message from first interrogated station",//9
            "Not used. Should be set to zero. Reserved for future use",//10
            "MMSI number of second interrogated station",//11
            "Requested message type from second interrogated station",//12
            "Response slot offset for requested message from second interrogated station",//13
            "Not used. Should be set to zero. Reserved for future use"//14
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_1,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.msg_1_1,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.slot_offset_1_1,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.spare2,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.dest_msg_1_2,1,1));
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.slot_offset_1_2,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.spare3,1,1));
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.mmsi_2,1,1));
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.msg_2,1,1));
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.slot_offset_2,1,1));
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.spare4,1,1));
    
    return AIS_OK;
}
*/

AIS_STATUS AIS16ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=11;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dest_mmsi_a", "offset_a", "inc_a", "dest_mmsi_b",
    "offset_b", "inc_b", "spare2"};
    mxArray *theStruct;
    
    Ais16 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 16; always 16.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of assigning station",//2
            "Spare. Should be set to zero. Reserved for future use",//3
            "MMSI number. Destination identifier A",//4
            "Offset from current slot to first assigned slot",//5
            "Increment to next assigned slot",//6
            "MMSI number. Destination identifier B. Should be omitted if there is assignment to station A, only",//7
            "Offset from current slot to first assigned slot. Should be omitted if there is assignment to station A, only",//8
            "Increment to next assigned slot. Should be omitted, if there is assignment to station A, only",//9
            "Spare. Not used. Should be set to zero. The number of spare bits, which should be 0 or 4, should be adjusted in order to observe byte boundaries. Reserved for future use"//10
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dest_mmsi_a,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.offset_a,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.inc_a,1,1));
    
    if(msg.dest_mmsi_b!=-1) {
        mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dest_mmsi_b,1,1));
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.offset_b,1,1));
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.inc_b,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.spare2,1,1));
        
    return AIS_OK;
}

AIS_STATUS AIS17ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=12;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "x", "y", "spare2", "gnss_type", "station", "z_cnt", "seq",
    "health"};
    mxArray *theStruct;
    
    Ais17 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
        
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 17; always 17.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of the base station",//2
            "Spare. Should be set to zero. Reserved for future use",//3
            "Surveyed longitude of DGNSS reference station in WGS-84 degrees",//4
            "Surveyed latitude of DGNSS reference station in WGS-84 degrees",//5
            "Not used. Should be set to zero. Reserved for future use",//6
            "GNSS Message Type as per ITU-R M.823",//7
            "Recommendation ITU-R M.823 station identifier",//8
            "Modified z-count in units of 0.6 seconds",//9
            "Message sequence number (cyclic 0-7)",//10
            "Reference station health (specified in Recommendation ITU-R M.823)"//11
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,4,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//No longitude is available
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,5,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//No latitude is available
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare2,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.gnss_type,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.station,1,1));
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.z_cnt,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.health,1,1));
    
    return AIS_OK;
}

AIS_STATUS AIS18ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=31;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "sog", "position_accuracy", "x", "y", "cog",
    "true_heading", "timestamp", "spare2", "unit_flag", "display_flag",
    "dsc_flag", "band_flag", "m22_flag", "mode_flag", "raim",
    "commstate_flag", "sync_state", "slot_timeout", "received_stations",
    "slot_number", "utc_hour", "utc_min", "utc_spare", "slot_offset",
    "slot_increment", "slots_to_allocate", "keep_flag"};
    mxArray *theStruct;
    
    Ais18 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 18; always 18.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "Speed over ground in knots. 102.2 means 102.2 knots or higher.",//4
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//5
            "Longitude East in degrees (WGS-84)",//6
            "Latitude North in degrees (WGS-84)",//7
            "Course over ground in degrees East of North",//8
            "True heading (the direction the ship is pointing/ would be going without wind/currents) in degrees East of North",//9
            "UTC second when report was generated or 61 if positioning system is in manual input mode or 62 if in dead reckining mode or 63 if positioning system is inoperative",//10
            "Not used. Should be set to zero. Reserved for future use.",//11
            "Class B unit flag\n0 = Class B self-organized time division multiple access (SOTDMA) unit\n1 = Class B carrier sense (CS) unit",//12
            "Class B display flag\n0 = No display available; not capable of displaying Message 12 and 14\n1 = Equipped with integrated display displaying Message 12 and 14",//13
            "Class B digital selective calling (DSC) flag\n0 = Not equipped with DSC function\n1 = Equipped with DSC function (dedicated or time-shared)",//14
            "Class B band flag\n0 = Capable of operating over the upper 525 kHz band of the marine band\n1 = Capable of operating over the whole marine band\n(irrelevant if Class B Message 22 flag is 0)",//15
            "Class B Message 22 flag\n0 = No frequency management via Message 22, operating on AIS 1, AIS 2 only\n1 = Frequency management via Message 22",//16
            "0 = Station operating in autonomous and continuous mode = default 1 = Station operating in assigned mode",//17
            "Receiver autonomous integrity monitoring (RAIM) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use.",//18
            "0 = SOTDMA communication state follows 1 = ITDMA communication state follows,(always '1' for Class-B CS)",//19
            "Synchronization state\n0 UTC direct\n1 UTC indirect\n2 Station is synchronized to a base station (base direct)\n3 Station is synchronized to another station based on the highest number of received stations or to another mobile station, which is directly synchronized to a base station",//20
            "(For SOTDMA) Specifies frames remaining until a new slot is selected\n0 means that this was the last transmission in this slot\n1-7 means that 1 to 7 frames respectively are left until slot change",//21
            "(For SOTDMA) Number of other stations (not own station) which the station currently is receiving (between 0 and 16 383).",//22
            "(For SOTDMA) Slot number used for this transmission (between 0 and 2 249).",//23
            "(For SOTDMA) Hour (0-23) in UTC time, if available",//24
            "(For SOTDMA) Minute (0-23) in UTC time, if available",//25
            "(For SOTDMA) Two extra (unused) bits that are associated with the time message",//26
            "(For SOTDMA) If the slot time-out value is 0 (zero) then the slot offset should indicate the offset to the slot in which transmission will occur during the next frame. If the slot offset is zero, the slot should be de-allocated after transmission.",//27
            "(For ITDMA) Offset to next slot to be used, or zero (0) if no more transmissions",//28
            "(For ITDMA) Number of consecutive slots to allocate.\n0 = 1 slot,\n1 = 2 slots,\n2 = 3 slots,\n3 = 4 slots,\n4 = 5 slots,\n5 = 1 slot; offset = slot increment + 8 192,\n6 = 2 slots; offset = slot increment + 8 192,\n7 = 3 slots; offset = slot increment + 8 192.\nUse of 5 to 7 removes the need for RATDMA broadcast for scheduled transmissions up to 6 min intervals",//29
            "(For ITDMA) Set to TRUE = 1 if the slot remains allocated for one additional frame"//30
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    
    if(msg.sog<=102.2) {
        mxSetFieldByNumber(theStruct,0,4,floatMat2MatlabDoubles(&msg.sog,1,1));
    } else {//If the speed over ground is unavailable or invalid
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//Longitude is not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//Latitude is not available
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.cog<360) {
        mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.cog,1,1));
    } else {//Course over ground is not available
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.true_heading<360) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.true_heading,1,1));
    } else {//True heading is not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.timestamp!=60) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.timestamp,1,1));
    } else {//Timestamp is not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.spare2,1,1));
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.unit_flag,1,1));
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.display_flag,1,1));
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.dsc_flag,1,1));
    mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.band_flag,1,1));
    mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.m22_flag,1,1));
    mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.mode_flag,1,1));
    mxSetFieldByNumber(theStruct,0,18,boolMat2Matlab(&msg.raim,1,1));
    mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.commstate_flag,1,1));

    mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.sync_state,1,1));
    if(msg.commstate_flag==0) {
        mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.slot_timeout,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.received_stations_valid) {
        mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.received_stations,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_number_valid) {
        mxSetFieldByNumber(theStruct,0,23,intMat2MatlabDoubles(&msg.slot_number,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_valid) {        
        mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.utc_hour,1,1));
        mxSetFieldByNumber(theStruct,0,25,intMat2MatlabDoubles(&msg.utc_min,1,1));
        mxSetFieldByNumber(theStruct,0,26,intMat2MatlabDoubles(&msg.utc_spare,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,26,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_offset_valid) {
        mxSetFieldByNumber(theStruct,0,27,intMat2MatlabDoubles(&msg.slot_offset,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,27,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_increment_valid) {
        mxSetFieldByNumber(theStruct,0,28,intMat2MatlabDoubles(&msg.slot_increment,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,28,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slots_to_allocate_valid) {
        mxSetFieldByNumber(theStruct,0,29,intMat2MatlabDoubles(&msg.slots_to_allocate,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,29,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.keep_flag_valid) {
        mxSetFieldByNumber(theStruct,0,30,boolMat2Matlab(&msg.keep_flag,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,30,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    return AIS_OK;
}

AIS_STATUS AIS19ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=23;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "sog", "position_accuracy", "x", "y", "cog",
    "true_heading", "timestamp", "spare2", "name", "type_and_cargo",
    "dim_a", "dim_b", "dim_c", "dim_d", "fix_type", "raim", "dte",
    "assigned_mode", "spare3"};
    mxArray *theStruct;
    
    Ais19 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
        
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 19; always 19.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "Speed over ground in knots. 102.2 means 102.2 knots or higher.",//4
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//5
            "Longitude East in degrees (WGS-84)",//6
            "Latitude North in degrees (WGS-84)",//7
            "Course over ground in degrees East of North",//8
            "True heading (the direction the ship is pointing/ would be going without wind/currents) in degrees East of North",//9
            "UTC second when report was generated or 61 if positioning system is in manual input mode or 62 if in dead reckining mode or 63 if positioning system is inoperative",//10
            "Not used. Should be set to zero. Reserved for future use.",//11
            "Name. Maximum 20 characters.",//12
            "Type of ship and cargo type\n1-99 = as defined in Section 3.3.2 of Annex 8 of ITU-R M.1371-5\n100-199 = reserved, for regional use 200-255 = reserved, for future use\nNot applicable to search and rescue (SAR) aircraft",//13
            "Distance from reference point to bow of ship (meters) 511m means 511m or longer. (If only relative reference point position known=0)",//14
            "Distance from reference point to aft of ship (meters) 511m means 511m or longer. ",//15
            "Distance from reference point to port side of ship (meters) 511m means 511m or longer. (If only relative reference point position known=0)",//16
            "Distance from reference point to starboard side of ship (meters) 511m means 511m or longer.",//17
            "Type of electronic position fixing device:\n1 = global positioning system (GPS)\n2 = GNSS (GLONASS)\n3 = combined GPS/GLONASS\n4 = Loran-C\n5 = Chayka\n6 = integrated navigation system\n7 = surveyed\n8 = Galileo\n9-14 = not used\n15 = internal GNSS",//18
            "Receiver autonomous integrity monitoring (RAIM) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use.",//19
            "Data terminal equipment (DTE) ready (0 = available, 1 = not available = default)",//20
            "0 = Station operating in autonomous and continuous mode = default 1 = Station operating in assigned mode",//21
            "Not used. Should be set to zero. Reserved for future use"//22
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    
     if(msg.sog<=102.2) {
        mxSetFieldByNumber(theStruct,0,4,floatMat2MatlabDoubles(&msg.sog,1,1));
    } else {//If the speed over ground is unavailable or invalid
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//Longitude is not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//Latitude is not available
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.cog<360) {
        mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.cog,1,1));
    } else {//Course over ground is not available
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.true_heading<360) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.true_heading,1,1));
    } else {//True heading is not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.timestamp!=60) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.timestamp,1,1));
    } else {//Timestamp is not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.spare2,1,1));

    if(msg.name.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.name.c_str();
        mxSetFieldByNumber(theStruct,0,12,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//If the name is not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.type_and_cargo!=0) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.type_and_cargo,1,1));
    } else {//No type and cargo information
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.dim_a==0&&msg.dim_b==0&&msg.dim_c==0&&msg.dim_d==0) {
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    } else {//If ship dimensions from reference point are available.
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.dim_a,1,1));
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.dim_b,1,1));
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.dim_c,1,1));
        mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.dim_d,1,1));
    }
    
    if(msg.fix_type!=0) {
        mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.fix_type,1,1));
    } else {//If the fix type is not available
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,19,boolMat2Matlab(&msg.raim,1,1));
    mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.dte,1,1));
    mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.assigned_mode,1,1));
    mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.spare3,1,1));

    return AIS_OK;
}

AIS_STATUS AIS20ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=21;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "offset_1", "num_slots_1", "timeout_1", "incr_1",
    "offset_2", "num_slots_2", "timeout_2", "incr_2", "offset_3",
    "num_slots_3", "timeout_3", "incr_3", "offset_4", "num_slots_4",
    "timeout_4", "incr_4", "spare2"};
    mxArray *theStruct;
    
    Ais20 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
      
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 20; always 20.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of base station",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "Reserved offset number 1",//4
            "Number of reserved consecutive slots 1: 1-15",//5
            "Time-out value 1 in minutes",//6
            "Increment to repeat reservation block 1; 0 = one reservation block per frame",//7
            "Reserved offset number 2 (optional)",//8
            "Number of reserved consecutive slots 2: 1-15; optional",//9
            "Time-out value 2 in minutes (optional)",//10
            "Increment to repeat reservation block 2 (optional)",//11
            "Reserved offset number 3 (optional)",//12
            "Number of reserved consecutive slots 3: 1-15; optional",//13
            "Time-out value 3 in minutes (optional)",//14
            "Increment to repeat reservation block 3 (optional)",//15
            "Reserved offset number 4 (optional)",//16
            "Number of reserved consecutive slots 4: 1-15; optional",//17
            "Time-out value 4 in minutes (optional)",//18
            "Increment to repeat reservation block 4 (optional)",//19
            "Not used. Should be set to zero. The number of spare bits which may be 0, 2, 4 or 6 should be adjusted in order to observe byte boundaries. Reserved for future use"//20
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    if(msg.offset_1!=0) {
        mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.offset_1,1,1));
    } else {//If the offset number is not available
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.num_slots_1) {
        mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.num_slots_1,1,1));
    } else {//If the number of slots is not available
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.timeout_1!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.timeout_1,1,1));
    } else {//If the timeout number is not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.incr_1,1,1));

    if(msg.group_valid_2) {
        if(msg.offset_2!=0) {
            mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.offset_2,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.num_slots_2!=0) {
            mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.num_slots_2,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.timeout_2!=0) {
            mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.timeout_2,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.incr_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.group_valid_3) {
        if(msg.offset_3!=0) {
            mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.offset_3,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.num_slots_3!=0) {
            mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.num_slots_3,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.timeout_3!=0) {
            mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.timeout_3,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
        }

        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.incr_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    
    if(msg.group_valid_4) {
        if(msg.offset_4!=0) {
            mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.offset_4,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.num_slots_4!=0) {
            mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.num_slots_4,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.timeout_4!=0) {
            mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.timeout_4,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.incr_4,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS21ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=21;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "aton_type", "name", "position_accuracy", "x", "y", "dim_a",
    "dim_b", "dim_c", "dim_d", "fix_type", "timestamp", "off_pos",
    "aton_status", "raim", "virtual_aton", "assigned_mode", "spare",
    "spare2"};
    mxArray *theStruct;
    
    Ais21 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
         
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 21; always 21.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number (see Article 19 of the RR and Recommendation ITU-R M.585)",//2
            "Type of aids-to-navigation (AtoN); refer to appropriate definition set up by IALA",
            "Name of AtoN, including any extension",
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//5
            "Longitude East in degrees (WGS-84)",//6
            "Latitude North in degrees (WGS-84)",//7
            "Distance from reference point to true North of structure (meters) 1 means less than 2 meters",//8
            "Distance from reference point to South of structure (meters) 1 means less than 2 meters",//9
            "Distance from reference point to West side of structure (meters) 1 means less than 2 meters",//10
            "Distance from reference point to East side of structure (meters) 1 means less than 2 meters",//11
            "Type of electronic position fixing device:\n1 = GPS\n2 = GLONASS\n3 = Combined GPS/GLONASS\n4 = Loran-C\n5 = Chayka\n6 = Integrated Navigation System\n7 = surveyed. For fixed AtoN and virtual AtoN, the charted position should be used. The accurate position enhances its function as a radar reference target\n8 = Galileo\n9-14 = not used\n15 = internal GNSS",//12
            "UTC second when report was generated or 61 if positioning system is in manual input mode or 62 if in dead reckining mode or 63 if positioning system is inoperative",//13
            "Off-position indicator. For floating AtoN, only: 0 = on position; 1 = off position.",//14
            "Reserved for the indication of the AtoN status 00000000 = default",//15
            "RAIM (Receiver autonomous integrity monitoring) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use",//16
            "0 = default = real AtoN at indicated position; 1 = virtual AtoN, does not physically exist.",//17
            "0 = Station operating in autonomous and continuous mode = default 1 = Station operating in assigned mode",//18
            "Spare. Not used. Should be set to zero. Reserved for future use",//19
            "Spare. Should be set to zero. The number of spare bits should be adjusted in order to observe byte boundaries"//20
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    
    if(msg.aton_type!=0) {
        mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.aton_type,1,1));
    } else {//Aid to navigation type unavailable
        mxSetFieldByNumber(theStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.name.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.name.c_str();
        mxSetFieldByNumber(theStruct,0,4,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//If the name is not available
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//Longitude not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//Latitude not available
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.dim_a==0&&msg.dim_b==0&&msg.dim_c==0&&msg.dim_d==0) {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    } else {//If dimensions are available
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.dim_a,1,1));
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.dim_b,1,1));
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.dim_c,1,1));
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.dim_d,1,1));
    }
    
    if(msg.fix_type!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.fix_type,1,1));
    } else {//If the fix type is not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.timestamp!=60) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.timestamp,1,1));
    } else {//timestamp not available
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.timestamp<=59) {
        mxSetFieldByNumber(theStruct,0,14,boolMat2Matlab(&msg.off_pos,1,1));
    } else {//Off-position indicator is invalid
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.aton_status,1,1));
    mxSetFieldByNumber(theStruct,0,16,boolMat2Matlab(&msg.raim,1,1));
    mxSetFieldByNumber(theStruct,0,17,boolMat2Matlab(&msg.virtual_aton,1,1));
    mxSetFieldByNumber(theStruct,0,18,boolMat2Matlab(&msg.assigned_mode,1,1));
    mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS22ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=18;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "chan_a", "chan_b", "txrx_mode", "power_low", "x1",
    "y1", "x2", "y2", "dest_mmsi_1", "dest_mmsi_2", "chan_a_bandwidth",
    "chan_b_bandwidth", "zone_size", "spare2"};
    mxArray *theStruct;
    
    Ais22 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
         
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 22; always 22.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of base station",//2
            "Not used. Should be set to zero. Reserved for future use",//3
            "Channel number of 25 kHz simplex or simplex use of 25 kHz duplex in accordance with Recommendation ITU-R M.1084.",//4
            "Channel number of 25 kHz simplex or simplex use of 25 kHz duplex in accordance with Recommendation ITU-R M.1084.",//5
            "Tx/Rx mode\n0 = Tx A/Tx B, Rx A/Rx B (default)\n1 = Tx A, Rx A/Rx B\n2 = Tx B, Rx A/Rx B\n3-15: not used\nWhen the dual channel transmission is suspended by Tx/Rx mode command 1 or 2, the required reporting interval should be maintained using the remaining transmission channel",//6
            "Power: 0 = high (default), 1 = low",//7
            "Longitude (WGS-84) of area to which the assignment applies (degrees); upper right corner (North-East)",//8
            "Latitude (WGS-84) of area to which the assignment applies (degrees); upper right corner (North-East)",//9
            "Longitude (WGS-84) of area to which the assignment applies (degrees); lower left corner (South-West)",//10
            "Latitude (WGS-84) of area to which the assignment applies (degrees); lower left corner (South-West)",//11
            "MMSI of addressed station 1",//12
            "MMSI of addressed station 2",//13
            "Channel A bandwidth:\n0 = default (as specified by channel number);\n1 = spare (formerly 12.5 kHz bandwidth in Recommendation ITU-R M.1371-1 now obsolete)",//14
            "Channel B bandwidth:\n0 = default (as specified by channel number);\n1 = spare (formerly 12.5 kHz bandwidth in Recommendation ITU-R M.1371-1 now obsolete)",//15
            "The transitional zone size in nautical miles should be calculated by adding 1 to this parameter value.",//16
            "Not used. Should be set to zero. Reserved for future use"//17
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.chan_a,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.chan_b,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.txrx_mode,1,1));
    mxSetFieldByNumber(theStruct,0,7,boolMat2Matlab(&msg.power_low,1,1));

    if(msg.pos_valid) {
        if(fabs(msg.x1)<=180) {
            mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.x1,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(fabs(msg.y1)<=90) {
            mxSetFieldByNumber(theStruct,0,9,floatMat2MatlabDoubles(&msg.y1,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(fabs(msg.x2)<=180) {
            mxSetFieldByNumber(theStruct,0,10,floatMat2MatlabDoubles(&msg.x2,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(fabs(msg.y2)<=90) {
            mxSetFieldByNumber(theStruct,0,11,floatMat2MatlabDoubles(&msg.y2,1,1));
        } else {
            mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.dest_valid) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.dest_mmsi_1,1,1));
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.dest_mmsi_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.chan_a_bandwidth,1,1));
    mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.chan_b_bandwidth,1,1));
    mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.zone_size,1,1));
    mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS23ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=15;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "x1", "y1", "x2", "y2", "station_type",
    "type_and_cargo", "spare2", "txrx_mode", "interval_raw", "quiet",
    "spare3"};
    mxArray *theStruct;
    
    Ais23 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
             
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 23; always 23.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of assigning station",//2
            "Spare. Should be set to zero",//3
            "Longitude (WGS-84) of area to which the assignment applies (degrees); upper right corner (North-East)",//4
            "Latitude (WGS-84) of area to which the assignment applies (degrees); upper right corner (North-East)",//5
            "Longitude (WGS-84) of area to which the assignment applies (degrees); lower left corner (South-West)",//6
            "Latitude (WGS-84) of area to which the assignment applies (degrees); lower left corner (South-West)",//7
            "Station type:\n0 = all types of mobiles (default)\n1 = Class A mobile stations only\n2 = all types of Class B mobile stations\n3 = SAR airborne mobile station\n4 = Class B 'SO' mobile stations only\n5 = Class B 'CS' shipborne mobile station only\n6 = inland waterways\n7 to 9 = regional use\n10 = base station coverage area (see Message 4 and Message 27)\n11 to 15 = for future use",//8
             "Type of ship and cargo type\n1-99 = as defined in Section 3.3.2 of Annex 8 of ITU-R M.1371-5\n100-199 = reserved, for regional use 200-255 = reserved, for future use",//9
            "Not used. Should be set to zero. Reserved for future use",//10
            "This parameter commands the respective stations to one of the following modes:\n0 = TxA/TxB, RxA/RxB (default)\n1 = TxA, RxA/RxB\n2 = TxB, RxA/RxB\n3 = reserved for future use",//11
             "This parameter commands the respective stations to the reporting interval given in Table 77 of ITU-R M.1371-5",//12
             "0 = default = no quiet time commanded; 1-15 = quiet time of 1 to 15 min",//3
             "Not used. Should be set to zero. Reserved for future use"//14
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,floatMat2MatlabDoubles(&msg.x1,1,1));
    mxSetFieldByNumber(theStruct,0,5,floatMat2MatlabDoubles(&msg.y1,1,1));
    mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x2,1,1));
    mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y2,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.station_type,1,1));
    
    if(msg.type_and_cargo!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.type_and_cargo,1,1));
    } else {//No type and cargo information
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }    
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.spare2,1,1));

    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.txrx_mode,1,1));
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.interval_raw,1,1));
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.quiet,1,1));
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.spare3,1,1));

    return AIS_OK;
}

AIS_STATUS AIS24ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=13;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "part_num", "name", "type_and_cargo", "vendor_id", "callsign",
    "dim_a", "dim_b", "dim_c", "dim_d", "spare"};
    mxArray *theStruct;

    Ais24 msg(nmea_payload, pad);
    
    if (msg.had_error()) {
        return msg.get_error();
    }
                 
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 24; always 24.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number",//2
            "Identifier for the message part number; always 0 for Part A, 1 for Part B",//3
            "Name of the MMSI-registered vessel. Maximum 20 characters 6-bit ASCII.",//4
            "Type of ship and cargo type\n1-99 = as defined in Section 3.3.2 of Annex 8 of ITU-R M.1371-5\n100-199 = reserved, for regional use 200-255 = reserved, for future use\nNot applicable to search and rescue (SAR) aircraft",//5
            "Unique identification of the Unit by a number as defined by the manufacturer",//6
            "Call sign. Craft associated with a parent vessel, should use 'A' followed by the last 6 digits of the MMSI of the parent vessel. Examples of these craft include towed vessels, rescue boats, tenders, lifeboats and liferafts.",//7
            "Distance from reference point to bow of ship (meters) 511m means 511m or longer. (If only relative reference point position known=0)",//8
            "Distance from reference point to aft of ship (meters) 511m means 511m or longer. ",//9
            "Distance from reference point to port side of ship (meters) 511m means 511m or longer. (If only relative reference point position known=0)",//10
            "Distance from reference point to starboard side of ship (meters) 511m means 511m or longer.",//11
            "Spare. Should be set to zero",//12     
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.part_num,1,1));
    if(msg.part_num==0) {
        //Only a Name
        if(msg.name.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
            const char *charString=msg.name.c_str();
            mxSetFieldByNumber(theStruct,0,4,mxCreateCharMatrixFromStrings(1,&charString));
        } else {//If the name has actually been set
            mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    } else if(msg.part_num==1) {
        //No Name 
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));//No name
        
        if(msg.type_and_cargo!=0) {
            mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.type_and_cargo,1,1));
        } else {//If the cargo type is not available
            mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.vendor_id.compare("@@@@@@@")!=0) {
            const char *charString=msg.vendor_id.c_str();
            mxSetFieldByNumber(theStruct,0,6,mxCreateCharMatrixFromStrings(1,&charString));
        } else {//If no vendor ID is set
            mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.callsign.compare("@@@@@@@")!=0) {
            const char *charString=msg.callsign.c_str();
            mxSetFieldByNumber(theStruct,0,7,mxCreateCharMatrixFromStrings(1,&charString));
        } else {//If no callsign is set
            mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.dim_a==0&&msg.dim_b==0&&msg.dim_c==0&&msg.dim_d==0) {
            mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
            mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
            mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
            mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
        } else {//If the dimensions are actually given
            mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.dim_a,1,1));
            mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.dim_b,1,1));
            mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.dim_c,1,1));
            mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.dim_d,1,1));
        }
        
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.spare,1,1));
    } else {
        //Undefined part_num value.
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    return AIS_OK;
}

AIS_STATUS AIS25ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=6;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "dest_mmsi", "dac", "fi"};
    mxArray *theStruct;
    
    Ais25 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
             
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 25; always 25.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Destination MMSI, if used",//3
            "Designated area code",//4
            "Function identifier"//5
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    if(msg.dest_mmsi_valid) {
        mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.dest_mmsi,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.use_app_id) {
        mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
        mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    return AIS_OK;
}

AIS_STATUS AIS26ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=18;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "dest_mmsi", "dac", "fi", "commstate_flag", "sync_state",
    "slot_timeout", "received_stations", "slot_number", "utc_hour",
    "utc_min", "utc_spare", "slot_offset", "slot_increment",
    "slots_to_allocate", "keep_flag"};
    mxArray *theStruct;
    
    Ais26 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
             
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 26; always 26.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Destination MMSI, if used",//3
            "Designated area code",//4
            "Function identifier",//5
            "0 = SOTDMA communication state follows 1 = ITDMA communication state follows",//6
            "Synchronization state\n0 UTC direct\n1 UTC indirect\n2 Station is synchronized to a base station (base direct)\n3 Station is synchronized to another station based on the highest number of received stations or to another mobile station, which is directly synchronized to a base station",//7
            "(For SOTDMA) Specifies frames remaining until a new slot is selected\n0 means that this was the last transmission in this slot\n1-7 means that 1 to 7 frames respectively are left until slot change",//8
            "(For SOTDMA) Number of other stations (not own station) which the station currently is receiving (between 0 and 16 383).",//9
            "(For SOTDMA) Slot number used for this transmission (between 0 and 2 249).",//10
            "(For SOTDMA) Hour (0-23) in UTC time, if available",//11
            "(For SOTDMA) Minute (0-23) in UTC time, if available",//12
            "(For SOTDMA) Two extra (unused) bits that are associated with the time message",//13
            "(For SOTDMA) If the slot time-out value is 0 (zero) then the slot offset should indicate the offset to the slot in which transmission will occur during the next frame. If the slot offset is zero, the slot should be de-allocated after transmission.",//14
            "(For ITDMA) Offset to next slot to be used, or zero (0) if no more transmissions",//15
            "(For ITDMA) Number of consecutive slots to allocate.\n0 = 1 slot,\n1 = 2 slots,\n2 = 3 slots,\n3 = 4 slots,\n4 = 5 slots,\n5 = 1 slot; offset = slot increment + 8 192,\n6 = 2 slots; offset = slot increment + 8 192,\n7 = 3 slots; offset = slot increment + 8 192.\nUse of 5 to 7 removes the need for RATDMA broadcast for scheduled transmissions up to 6 min intervals",//16
            "(For ITDMA) Set to TRUE = 1 if the slot remains allocated for one additional frame"//17
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    
    if(msg.dest_mmsi_valid) {
        mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.dest_mmsi,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.use_app_id) {
        mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
        mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,4,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.commstate_flag,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.sync_state,1,1));

    if(msg.slot_timeout_valid) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.slot_timeout,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.received_stations_valid) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.received_stations,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_number_valid) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.slot_number,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_valid) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_hour,1,1));
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_min,1,1));
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_spare,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_offset_valid) { 
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.slot_offset,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slot_increment_valid) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.slot_increment,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.slots_to_allocate_valid) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.slots_to_allocate,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.keep_flag_valid) {
        mxSetFieldByNumber(theStruct,0,17,boolMat2Matlab(&msg.keep_flag,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    return AIS_OK;
}

AIS_STATUS AIS27ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
    const size_t numberOfFields=12;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "position_accuracy", "raim", "nav_status", "x", "y", "sog",
    "cog", "gnss", "spare"};
    mxArray *theStruct;
    
    Ais27 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
               
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 27; always 27.",//0
            "Repeat indicator. Always 3",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//6
            "Receiver autonomous integrity monitoring (RAIM) flag of electronic position fixing device; 0 = RAIM not in use = default; 1 = RAIM in use.",//14
            "0=under way using engine\n1=at anchor,\n2=not under command\n3=restricted maneuverability\n4=constrained by her draught\n5=moored\n6=aground\n7=engaged in fishing\n8=under way sailing\n9=reserved for future amendment of navigational status for ships carrying DG, HS, or MP, or IMO hazard or pollutant category C, high speed craft (HSC)\n10=reserved for future amendment of navigational status for ships carrying dangerous goods (DG), harmful substances (HS) or marine pollutants (MP), or IMO hazard or pollutant category A, wing in ground (WIG)\n11=power-driven vessel towing astern (regional use)\n12=power-driven vessel pushing ahead or towing alongside (regional use)\n13 = reserved for future use\n14=AIS-SART (active), MOB-AIS, EPIRB-AIS\n15=undefined = default (also used by AIS-SART, MOB-AIS and EPIRB- AIS under test)",//3
            "Longitude East in degrees (WGS-84)",//7
            "Latitude North in degrees (WGS-84)",//8
            "Speed over ground in knots",//5
            "Course over ground in degrees East of North",//9
            "1=Reported position latency is less than 5 seconds, 0=Reported position latency is greater than five seconds",//10
            "Set to zero, to preserve byte boundaries"//11
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));

    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    mxSetFieldByNumber(theStruct,0,4,boolMat2Matlab(&msg.raim,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.nav_status,1,1));
    
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//If longitude not available
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.y<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//If latitude not available
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.sog,1,1));
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.cog,1,1));
    mxSetFieldByNumber(theStruct,0,10,boolMat2Matlab(&msg.gnss,1,1));
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.spare,1,1));
   
    return AIS_OK;
}

//These are all of the functions for AIS6
AIS_STATUS AIS_FI_6_1_0_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in Annex 5 of ITU-R M.1371-5.
    const size_t numberOfFields=13;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "ack_required", "msg_seq", "text", "spare2"};
    mxArray *theStruct;
    
    Ais6_1_0 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "Acknowledge required flag\n1 = reply is required, optional for addressed binary messages and not used for binary broadcast messages\n0 = reply is not required, optional for an addressed binary message and required for binary broadcast messages",//9
             "Sequence number to be incremented by the application. All zeros indicates that sequence numbers are not being used",//10
             "Text string",//11
             "Not used for data and should be set to zero. The number of bits should be either 0, 2, 4, or 6 to maintain byte boundaries."//12
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais6_1_0 type.
    mxSetFieldByNumber(theStruct,0,9,boolMat2Matlab(&msg.ack_required,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.msg_seq,1,1));
    {
        const char *charString=msg.text.c_str();
        mxSetFieldByNumber(theStruct,0,11,mxCreateCharMatrixFromStrings(1,&charString));
    }
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_1_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in Annex 5 of ITU-R M.1371-1. It is not
//present in ITU-R M.1371-5
    const size_t numberOfFields=12;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "ack_dac", "msg_seq", "spare2"};
    mxArray *theStruct;
    
    Ais6_1_1 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "DAC being acknowledged",//9
            "Sequence number of the message being acknowledged",//10
            "Not used. Should be zero."//11
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais6_1_1 type.
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.ack_dac,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.msg_seq,1,1));
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}


AIS_STATUS AIS_FI_6_1_2_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in Annex 5 of ITU-R M.1371-5.
    const size_t numberOfFields=11;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "req_dac", "req_fi"};
    mxArray *theStruct;
    
    Ais6_1_2 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
        
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "Requested DAC: International application identifier (IAI), regional application identifier (RAI) or test",//9
            "Requested FI",//10
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_2 type.
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.req_dac,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.req_fi,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_3_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in Annex 5 of ITU-R M.1371-5.
    const size_t numberOfFields=11;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "req_dac", "spare2"};
    mxArray *theStruct;
    
    Ais6_1_3 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
        
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "Requested DAC: International application identifier (IAI), regional application identifier (RAI) or test",//9
            "Not used, should be set to zero, reserved for future use"//10
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_3 type.
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.req_dac,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_4_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in Annex 5 of ITU-R M.1371-5.
    const size_t numberOfFields=13;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "ack_dac", "capabilities", "cap_reserved", "spare2"};
    mxArray *theStruct;
    
    Ais6_1_4 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
            
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "DAC code replied",//9
            "A 64X1 array of boolean values, where a 1 indicates whether the FI value for that position is available. In Matlab, capabilities(1) corresponds to FI=0",//10
            "A 64X1 array of values reserved for future use.",//11
            "Not used, should be set to zero, reserved for future use"//12
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_4 type.
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.ack_dac,1,1));
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(msg.capabilities,64,1));
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(msg.cap_reserved,64,1));
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_40_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in Annex 5 of ITU-R M.1371-1. It is not
//present in ITU-R M.1371-5.
    const size_t numberOfFields=11;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "persons", "spare2"};
    mxArray *theStruct;

    Ais6_1_40 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "Current number of persons on board, including crew members",//9
            "Not used. Should be set to zero"//10
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
 
        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_40 type.
    if(msg.persons!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.persons,1,1));
    } else {//The number of persons is not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}
/*
AIS_STATUS AIS_FI_6_1_12_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 236, which was revoked
//in IMO circular 289 in 2013.
    const size_t numberOfFields=25;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "last_port", "utc_month_dep", "utc_day_dep", "utc_hour_dep",
    "utc_min_dep", "next_port", "utc_month_next", "utc_day_next",
    "utc_hour_next", "utc_min_next", "main_danger", "imo_cat", "un",
    "value", "value_unit", "spare2"};
    mxArray *theStruct;
    
    Ais6_1_12 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "The United Nations (UN) locode of the last port of call.",//9
            "UTC month of departure (1-12) from last part of call",//10
            "UTC day of departure (1-31) from last part of call",//11
            "UTC hour of departure (0-23) from last part of call",//12
            "UTC minute of departure (0-59) from last part of call",//13
            "The UN locode of the next port of call.",//14
            "UTC estimated time of arrival (ETA) Month (1-12)",//15
            "UTC ETA day of departure (1-31)",//16
            "UTC ETA hour of departure (0-23)",//17
            "UTC ETA minute of departure (0-59)",//18
            "Main dangeroud good",//19
            "IMD category of main dangerous good",//20
            "UN Number (1-3363) of main dangeroud good",//21
            "Quantity of main dangerous good (1-1023) --Units specified in the next field",//22
            "Units of quantity of main dangerous good\n1 = in kg\n2 = in tons (10e3 kg)\n3 = in 1000 tons (10e6 kg)",//23
            "Not used. Should be set to zero"//24
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_12 type.
    if(msg.last_port.compare("@@@@@")!=0) {
        const char *charString=msg.last_port.c_str();
        mxSetFieldByNumber(theStruct,0,9,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//If the last part of call is not available.
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.utc_month_dep!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_month_dep,1,1));
    } else {//Month of departure not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_day_dep!=0) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_day_dep,1,1));
    } else {//Day of departure not available
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_hour_dep<24) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_hour_dep,1,1));
    } else {//Hour of departure not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_min_dep<60) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_min_dep,1,1));
    } else {//Minute of departure not available
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.next_port.compare("@@@@@")!=0) {
        const char *charString=msg.next_port.c_str();
        mxSetFieldByNumber(theStruct,0,14,mxCreateCharMatrixFromStrings(1,&charString)); 
    } else {//Next port of call is not available
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_month_next!=0) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.utc_month_next,1,1));
    } else {//Month of departure not available
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_day_next!=0) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.utc_day_next,1,1));
    } else {//Day of departure not available
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_hour_next<24) {
        mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.utc_hour_next,1,1));
    } else {//Hour of departure not available
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_min_next<60) {
        mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.utc_min_next,1,1));
    } else {//Minute of departure not available
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.main_danger.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.main_danger.c_str();
        mxSetFieldByNumber(theStruct,0,19,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//Main dangerous good name not available
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.imo_cat.compare("@@@@@")!=0) {
        const char *charString=msg.imo_cat.c_str();
        mxSetFieldByNumber(theStruct,0,20,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//IMD Category of main dangerous good not available
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.un!=0) {
        mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.un,1,1));
    } else {//UN Number of main dangeroud good not available
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.value!=0) {
        mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.value,1,1));
    } else {//Quantity of main dangerous good not available
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.value_unit!=0) {
        mxSetFieldByNumber(theStruct,0,23,intMat2MatlabDoubles(&msg.value_unit,1,1));
    } else {//Units of quantity of main dangeroud good not available
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}*/

AIS_STATUS AIS_FI_6_1_14_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 236, which was revoked
//in IMO circular 289 in 2013.
    const size_t numberOfFields=12;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "utc_month", "utc_day", "windows"};
    const size_t numberOfWindowFields=8;
    mwSize windowDims[2];
    const char *windowFieldNames[numberOfWindowFields] = {"x", "y", "utc_hour_from",
    "utc_min_from", "utc_hour_to", "utc_min_to", "cur_dir", "cur_speed"};
    mxArray *theStruct, *windowArray;
    size_t curWindow;
    
    Ais6_1_14 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
        
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "UTC Month (1-12)",//9
            "UTC Day (1-31)",//10
            "An array of tidal windows at different times consisting of components:\nLongitude East in degrees (WGS-84)\nLatitude North in degrees (WGS-84)\nUTC hours from (0-23)\nUTC minutes from (0-59)\nUTC hours to (0-23)\nUTC minutes to (0-59)\nDirection of the current in degrees East of North\nSpeed of the current in knots (0-12.6)"//11
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_14 type.
    if(msg.utc_month!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.utc_month,1,1));
    } else {//If UTC month is not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_day!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_day,1,1));
    } else {//If UTC day is not available.
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    windowDims[0]=msg.windows.size();
    windowDims[1]=1;
    windowArray=mxCreateStructArray(2, dims, numberOfWindowFields, windowFieldNames);
    mxSetFieldByNumber(theStruct,0,11,windowArray);

    for (curWindow = 0; curWindow < msg.windows.size(); curWindow++) {
        if(fabs(msg.windows[curWindow].x)<=180) {
            mxSetFieldByNumber(windowArray,curWindow,0,floatMat2MatlabDoubles(&msg.windows[curWindow].x,1,1));
        } else {//Longitude is not available
            mxSetFieldByNumber(windowArray,curWindow,0,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        if(fabs(msg.windows[curWindow].y)<=90) {
            mxSetFieldByNumber(windowArray,curWindow,1,floatMat2MatlabDoubles(&msg.windows[curWindow].y,1,1));
        } else {//Latitude is not available
            mxSetFieldByNumber(windowArray,curWindow,1,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        if(msg.windows[curWindow].utc_hour_from<24) {
            mxSetFieldByNumber(windowArray,curWindow,2,intMat2MatlabDoubles(&msg.windows[curWindow].utc_hour_from,1,1));
        } else {// Hour from is not available
            mxSetFieldByNumber(windowArray,curWindow,2,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.windows[curWindow].utc_min_from<60) {
            mxSetFieldByNumber(windowArray,curWindow,3,intMat2MatlabDoubles(&msg.windows[curWindow].utc_min_from,1,1));
        } else {//Minute from is not avaiable
            mxSetFieldByNumber(windowArray,curWindow,3,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.windows[curWindow].utc_hour_to<24) {
            mxSetFieldByNumber(windowArray,curWindow,4,intMat2MatlabDoubles(&msg.windows[curWindow].utc_hour_to,1,1));
        } else {// Hour from is not available
            mxSetFieldByNumber(windowArray,curWindow,4,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.windows[curWindow].utc_min_to<60) {
            mxSetFieldByNumber(windowArray,curWindow,5,intMat2MatlabDoubles(&msg.windows[curWindow].utc_min_to,1,1));
        } else {//Minute from is not avaiable
            mxSetFieldByNumber(windowArray,curWindow,5,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.windows[curWindow].cur_dir<360) {
            mxSetFieldByNumber(windowArray,curWindow,6,intMat2MatlabDoubles(&msg.windows[curWindow].cur_dir,1,1));
        } else {//Direction of the current not available
            mxSetFieldByNumber(windowArray,curWindow,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.windows[curWindow].cur_speed!=12.7) {
            mxSetFieldByNumber(windowArray,curWindow,7,floatMat2MatlabDoubles(&msg.windows[curWindow].cur_speed,1,1));
        } else {//Speed of the current not available
            mxSetFieldByNumber(windowArray,curWindow,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
    }
    
    return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_18_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 289
    const size_t numberOfFields=19;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "link_id", "utc_month", "utc_day", "utc_hour", "utc_min", "port_berth",
    "dest", "x", "y", "spare2"};
    mxArray *theStruct;
    
    Ais6_1_18 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "A source specific running number (1-1023), unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID and the first six digits of the source MMSI uniquely identify the sent message.",//9
            "UTC clearance month to enter port (1-12)",//11
            "UTC clearance day to enter port (1-12)",//12
            "UTC clearance hour to enter port (1-12)",//13
            "UTC clearance minute to enter port (1-12)",//14
            "Name of the port and berth",//15
            "UN locode of the destination",//16
            "Longitude East in degrees (WGS-84)",//17
            "Latitude North in degrees (WGS-84)",//18
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_18 type.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {//No link ID
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_month!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_month,1,1));
    } else {//If UTC month is not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_day!=0) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_day,1,1));
    } else {//If UTC day is not available.
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_hour<24) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_hour,1,1));
    } else {//If UTC hour not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_min<60) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_min,1,1));
    } else {//If UTC minute not available
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.port_berth.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.port_berth.c_str();
        mxSetFieldByNumber(theStruct,0,14,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//if name of port and berth is not available
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.dest.compare("@@@@@")!=0) {
        const char *charString=msg.dest.c_str();
        mxSetFieldByNumber(theStruct,0,15,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//No UN destination locode avaiable
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,16,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//No longitude avaiable
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,17,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//No latitude available
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.spare2[1],2,1));
    
    return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_20_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 289
    const size_t numberOfFields=21;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "link_id", "length", "depth", "position", "utc_month", "utc_day",
    "utc_hour", "utc_min", "services", "name", "x", "y"};
    mxArray *theStruct;
    
    Ais6_1_20 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
      
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "A source specific running number (1-1023), unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID and the first six digits of the source MMSI uniquely identify the sent message.",//9
            "Berth length in meters. 1 - 510 metres, 511 = 511 metres or greater",//10
            "Water depth at berth in meters. 0.1-25.4 meters. 25.5= 25.5 meters or deeper",//11
            "Mooring Position:\n1 = port-side to\n2 = starboard-side to\n3 = Mediterranean mooring\n4 = mooring buoy\n5 = anchorage\n6 - 7 (reserved for future use)",//12
            "Berth Date/ Time UTC month (1-12)",//13
            "Berth Date/ Time UTC day (1-31)",//14
            "Berth Date/ Time UTC hour (0-23)",//15
            "Berth Date/ Time UTC minute (0-59)",//16
            "A 26X1 vector of values indicating the availability of services specified in IMO circular 289. For each entry:\n0 = service not available or requested = default\n1 = service available\n2 = no data or unknown\n3 = not to be used",//17
            "The name of the berth",//18
            "Central longitude East of the berth in degrees (WGS-84)",//19
            "Central latitude North of the berth in degrees (WGS-84)",//20        
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_20 type.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {//No link ID
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.length!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.length,1,1));
    } else {//No berth length
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.depth!=0) {
        mxSetFieldByNumber(theStruct,0,11,floatMat2MatlabDoubles(&msg.depth,1,1));
    } else {//No berth depth
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.position!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.position,1,1));
    } else {//No mooring position specified
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_month!=0) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_month,1,1));
    } else {//If UTC month is not available
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_day!=0) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.utc_day,1,1));
    } else {//If UTC day is not available.
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_hour<24) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.utc_hour,1,1));
    } else {//If UTC hour not available
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_min<60) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.utc_min,1,1));
    } else {//If UTC minute not available
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    if(msg.services_known) {
        mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(msg.services,26,1));
    } else {
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.name.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.name.c_str();
        mxSetFieldByNumber(theStruct,0,18,mxCreateCharMatrixFromStrings(1,&charString));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,19,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//No longitude available
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(fabs(msg.y)<=90){
        mxSetFieldByNumber(theStruct,0,20,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//No latitude available
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
    }

   return AIS_OK;
}

AIS_STATUS AIS_FI_6_1_25_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 289
    const size_t numberOfFields=12;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "amount_unit", "amount", "cargos"};
    const size_t numberOfCargoFields=7;
    mwSize cargoDims[2];
    const char *cargoFieldNames[numberOfCargoFields] = {"code_type",
    "imdg", "spare", "un", "bc", "marpol_oil", "marpol_cat"};
    mxArray *theStruct, *cargoArray;
    size_t curCargo;
    
    Ais6_1_25 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
         
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
             "Units of quantity of main dangerous good\n1 = in kg\n2 = in tons (10e3 kg)\n3 = in 1000 tons (10e6 kg)",//9
            "Quantity of dangerous cargo (1-1023)",//10
            ""//11 Not used; a structure indicating each of the subelements is saved instead.
        };
        const char *cargoDescriptionStrings[numberOfCargoFields] ={
            "Code under which the cargo is carried. Values are:\n1 = IMDG Code (in packed form)\n2 = IGC Code\n3 = BC Code (from 1.1.2011 IMSBC)\n4 = MARPOL Annex I List of oils (Appendix 1) 5 = MARPOL Annex II IBC Code\n6 = Regional use\n7 - 15 (reserved for future use)",//0
            "IMDG code of the cargo:\n1 - 9 (not used)\n10 - 99 = first digit = main class, second digit = subclass or division (undefined subclasses and divisions should not be used)\n100 - 127 (reserved for future use)",//1
            "Not used. Set to zero.",//2
            "UN Number of cargo, 1 - 3363 = Four digits UN number 3364 - 8191 (reserved for future use)",//3
            "BC code of cargo:\n1=A\n2=B\n3=C\n4=MHB - Material Hazardous in Bulk\n5 - 7 (reserved for future use)",//4
            "Marpol Oil Type:\n1 = asphalt solutions\n2 = oils\n3 = distillates\n4 = gas oil\n5 = gasoline blending stocks\n6 = gasoline\n7 = jet fuels\n8 = naphtha\n9 - 15 (reserved for future use)",//5
            "Marpol category:\n1 = Category X\n2 = Category Y\n3 = Category Z\n4 = other substances\n5 - 7 (reserved for future use)"//6
        };
        int i;
        mxArray *cargoDescripStruct;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
        for(i=0;i<(numberOfFields-1);i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
        cargoDescripStruct=mxCreateStructArray(2, dims, numberOfCargoFields, cargoFieldNames);
        for(i=0;i<(numberOfCargoFields-1);i++) {
            mxSetFieldByNumber(cargoDescripStruct,0,i,mxCreateCharMatrixFromStrings(1,&cargoDescriptionStrings[i]));
        }
        //Set the descriptions for the cargo fields
        mxSetFieldByNumber(*fieldDescriptions,0,(numberOfFields-1),cargoDescripStruct);
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_25 type.
    if(msg.amount_unit!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.amount_unit,1,1));
    } else {//Dangerous cargo units not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.amount!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.amount,1,1));
    } else {//Dangerous cargo amount not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    cargoDims[0]=msg.cargos.size();
    cargoDims[1]=1;
    cargoArray=mxCreateStructArray(2, dims, numberOfCargoFields, cargoFieldNames);
    mxSetFieldByNumber(theStruct,0,11,cargoArray);
    
    for (curCargo= 0; curCargo < msg.cargos.size(); curCargo++) {
        if(msg.cargos[curCargo].code_type!=0) {
            mxSetFieldByNumber(cargoArray,curCargo,0,intMat2MatlabDoubles(&msg.cargos[curCargo].code_type,1,1));
        } else {//Code type not available
            mxSetFieldByNumber(cargoArray,curCargo,0,mxCreateDoubleMatrix(0,0,mxREAL));
        }

        if(msg.cargos[curCargo].imdg_valid&&msg.cargos[curCargo].imdg!=0) {
            mxSetFieldByNumber(cargoArray,curCargo,1,intMat2MatlabDoubles(&msg.cargos[curCargo].imdg,1,1));
        } else {
            mxSetFieldByNumber(cargoArray,curCargo,1,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.cargos[curCargo].spare_valid) {
            mxSetFieldByNumber(cargoArray,curCargo,2,intMat2MatlabDoubles(&msg.cargos[curCargo].spare,1,1));
        } else {
            mxSetFieldByNumber(cargoArray,curCargo,2,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.cargos[curCargo].un_valid&&msg.cargos[curCargo].un!=0) {
            mxSetFieldByNumber(cargoArray,curCargo,3,intMat2MatlabDoubles(&msg.cargos[curCargo].un,1,1));
        } else {
            mxSetFieldByNumber(cargoArray,curCargo,3,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.cargos[curCargo].bc_valid&&msg.cargos[curCargo].bc!=0) {
            mxSetFieldByNumber(cargoArray,curCargo,4,intMat2MatlabDoubles(&msg.cargos[curCargo].bc,1,1));
        } else {
            mxSetFieldByNumber(cargoArray,curCargo,4,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.cargos[curCargo].marpol_oil_valid&&msg.cargos[curCargo].marpol_oil!=0) {
            mxSetFieldByNumber(cargoArray,curCargo,5,intMat2MatlabDoubles(&msg.cargos[curCargo].marpol_oil,1,1));
        } else {
            mxSetFieldByNumber(cargoArray,curCargo,5,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.cargos[curCargo].marpol_cat_valid&&msg.cargos[curCargo].marpol_cat!=0) {
            mxSetFieldByNumber(cargoArray,curCargo,6,intMat2MatlabDoubles(&msg.cargos[curCargo].marpol_cat,1,1));
        } else {
            mxSetFieldByNumber(cargoArray,curCargo,6,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    }

    return AIS_OK;
}
/*
AIS_STATUS AIS_FI_6_1_28_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad){
    //This message format is described in IMO circular 289
    const size_t numberOfFields=18;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "link_id", "sender_type", "route_type", "utc_month_start",
    "utc_day_start", "utc_hour_start", "utc_min_start", "duration",
    "waypoints"};
    const size_t numberOfWaypointFields=2;
    mwSize waypointDims[2];
    const char *waypointFieldNames[numberOfWaypointFields] = {"x", "y"};
    mxArray *theStruct, *waypointArray;
    size_t curWaypoint;
    
    Ais6_1_28 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
  
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "A source specific running number (1-1023), unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID and the first six digits of the source MMSI uniquely identify the sent message.",//9
            "0 = ship = default, 1 = authority, 2 - 7 (reserved for future use)",//10
            "Route type\n1 = mandatory route\n2 = recommended route\n3 = alternative route\n4 = recommended route through ice\n5 = ship route plan\n6 - 30 (reserved for future use)\n31 = cancellation (cancel route as identified by Message Linkage ID)",//11
            "UTC start month (1-12)",//12
            "UTC start day (1-31)",//13
            "UTC start hour (0-23)",//14
            "UTC start minute (0-59)",//15
            "Minutes until end of validity of the route from start date and time. 0= cancel route",//16
            "An array containing WGS-84 longitude East and latitude North coordinates in degrees of the waypoints"//17
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais6_1_28 type.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.sender_type,1,1));
    if(msg.route_type!=0) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.route_type,1,1));
    } else {//No route type available
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_month_start!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_month_start,1,1));
    } else {//Month of departure not available
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_day_start!=0) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_day_start,1,1));
    } else {//Day of departure not available
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_hour_start<24) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.utc_hour_start,1,1));
    } else {//Hour of departure not available
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.utc_min_start<60) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.utc_min_start,1,1));
    } else {//Minute of departure not available
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.duration!=262143) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.duration,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    waypointDims[0]=msg.waypoints.size();
    waypointDims[1]=1;
    waypointArray=mxCreateStructArray(2, waypointDims, numberOfWaypointFields, waypointFieldNames);
    mxSetFieldByNumber(theStruct,0,17,waypointArray);

    for(curWaypoint= 0; curWaypoint < msg.waypoints.size(); curWaypoint++) {
        if(fabs(msg.waypoints[curWaypoint].x)<=180) {
            mxSetFieldByNumber(waypointArray,curWaypoint,0,floatMat2MatlabDoubles(&msg.waypoints[curWaypoint].x,1,1));
        } else {//Longitude coordinate not available
            mxSetFieldByNumber(waypointArray,curWaypoint,0,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        if(fabs(msg.waypoints[curWaypoint].y)<=90){
            mxSetFieldByNumber(waypointArray,curWaypoint,1,floatMat2MatlabDoubles(&msg.waypoints[curWaypoint].y,1,1));
        } else {//Latitude coordinate not available
            mxSetFieldByNumber(waypointArray,curWaypoint,1,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    }

    return AIS_OK;
}
*/
/*
AIS_STATUS AIS_FI_6_1_30_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 289
    const size_t numberOfFields=11;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "link_id", "text"};
    mxArray *theStruct;
    
    Ais6_1_30 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
      
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "Message link ID used to link the text description message with a main message. The connection ID and source MMSI MID uniquely identifies the main message. (1-1023)",
            "Test string, often from IMO Standard Marine Communication Phrases (SMCP)"//10
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_30 type.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {//If there is no link ID
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    {
        const char *charString=msg.text.c_str();
        mxSetFieldByNumber(theStruct,0,10,mxCreateCharMatrixFromStrings(1,&charString));
    }

    return AIS_OK;
}
*/

AIS_STATUS AIS_FI_6_1_32_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad){
//This message format is described in IMO circular 289
    const size_t numberOfFields=12;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi",
    "utc_month", "utc_day", "windows"};
    const size_t numberOfWindowFields=8;
    mwSize windowDims[2];
    const char *windowFieldNames[numberOfWindowFields] = {"x", "y", "from_utc_hour",
    "from_utc_min", "to_utc_hour", "to_utc_min", "cur_dir", "cur_speed"};
    mxArray *theStruct, *windowArray;
    size_t curWindow;
    
    Ais6_1_32 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
              
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
            "UTC Month (1-12)",//9
            "UTC Day (1-31)",//10
            "An array of tidal windows at different times consisting of components:\nLongitude East in degrees (WGS-84)\nLatitude North in degrees (WGS-84)\nUTC hours from (0-23)\nUTC minutes from (0-59)\nUTC hours to (0-23)\nUTC minutes to (0-59)\nDirection of the current in degrees East of North\nSpeed of the current in knots, 25.1 means 25.1 knots or greater"//11
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure
    //This first set is common to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais6_1_32 type.
    if(msg.utc_month!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.utc_month,1,1));
    } else {//No UTC month given
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_day!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_day,1,1));
    } else {//No UTC day given
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }

    windowDims[0]=msg.windows.size();
    windowDims[1]=1;
    windowArray=mxCreateStructArray(2, dims, numberOfWindowFields, windowFieldNames);
    mxSetFieldByNumber(theStruct,0,11,windowArray);

    for (curWindow = 0; curWindow < msg.windows.size(); curWindow++) {
        if(fabs(msg.windows[curWindow].x)<=180) {
            mxSetFieldByNumber(windowArray,curWindow,0,floatMat2MatlabDoubles(&msg.windows[curWindow].x,1,1));
        } else {//No longitude available
            mxSetFieldByNumber(windowArray,curWindow,0,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(fabs(msg.windows[curWindow].y)<=90) {
            mxSetFieldByNumber(windowArray,curWindow,1,floatMat2MatlabDoubles(&msg.windows[curWindow].y,1,1));
        } else {//No latitude available
            mxSetFieldByNumber(windowArray,curWindow,1,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.windows[curWindow].from_utc_hour<24) {
            mxSetFieldByNumber(windowArray,curWindow,2,intMat2MatlabDoubles(&msg.windows[curWindow].from_utc_hour,1,1));
        } else {//No UTC hour from available
            mxSetFieldByNumber(windowArray,curWindow,2,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.windows[curWindow].from_utc_min<60) {
            mxSetFieldByNumber(windowArray,curWindow,3,intMat2MatlabDoubles(&msg.windows[curWindow].from_utc_min,1,1));
        } else {//No UTC minutes from available
            mxSetFieldByNumber(windowArray,curWindow,3,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.windows[curWindow].to_utc_hour<24) {
            mxSetFieldByNumber(windowArray,curWindow,4,intMat2MatlabDoubles(&msg.windows[curWindow].to_utc_hour,1,1));
        } else {//No UTC hour to available
            mxSetFieldByNumber(windowArray,curWindow,4,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.windows[curWindow].to_utc_min<60) {
            mxSetFieldByNumber(windowArray,curWindow,5,intMat2MatlabDoubles(&msg.windows[curWindow].to_utc_min,1,1));
        } else {//No UTC minutes to avaiable
            mxSetFieldByNumber(windowArray,curWindow,5,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.windows[curWindow].cur_dir<360) {
            mxSetFieldByNumber(windowArray,curWindow,6,intMat2MatlabDoubles(&msg.windows[curWindow].cur_dir,1,1));
        } else {//No direction of the current avaiable
            mxSetFieldByNumber(windowArray,curWindow,6,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        
        if(msg.windows[curWindow].cur_speed!=255) {
            mxSetFieldByNumber(windowArray,curWindow,7,floatMat2MatlabDoubles(&msg.windows[curWindow].cur_speed,1,1));
        } else {//No current speed available
            mxSetFieldByNumber(windowArray,curWindow,7,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    }
    
    return AIS_OK;
}

AIS_STATUS AIS6ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad){
//This is if the specific subtype could not be identified. The fields are
//those common to all AIS 6 messages as described in ITU-R M.1371-5.
    const size_t numberOfFields=9;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "seq", "mmsi_dest", "retransmit", "spare", "dac", "fi"};
    mxArray *theStruct;
    
    Ais6 msg(nmea_payload, pad);
    if (msg.had_error()&&msg.get_error()!=AIS_UNINITIALIZED) {
        return msg.get_error();
    }
                
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 6; always 6.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Sequence number (0-3); refer to Section 5.3.1 of Annex 2 of ITU-R M.1371-5",
            "MMSI number of destination station",//4
            "Retransmit flag should be set upon retransmission: 0 = no retransmission = default; 1 = retransmitted",//5
            "Not used. Should be zero. Reserved for future use",//6
            "Designated area code (DAC)",//7
            "Function identifier (FI)",//8
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS6 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.seq,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.mmsi_dest,1,1));
    mxSetFieldByNumber(theStruct,0,5,boolMat2Matlab(&msg.retransmit,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.fi,1,1));
    
    return AIS_OK;
}

AIS_STATUS AIS_8_1_0_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message is described in Annex 5 of ITU-R M.1371-5.
    const size_t numberOfFields=10;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi", "ack_required", "msg_seq", "text",
    "spare2"};
    mxArray *theStruct;
    
    Ais8_1_0 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }

    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Acknowledge required flag\n1 = reply is required, optional for addressed binary messages and not used for binary broadcast messages\n0 = reply is not required, optional for an addressed binary message and required for binary broadcast messages",//6
            "Text sequence number to be incremented by the application. Zero indicates that sequence numbers are not being used",//7
            "Text String",//8
            "Not used for data and should be set to zero. The number of bits should be either 0, 2, 4, or 6 to maintain byte boundaries."//9
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais8_1_0 type.
    mxSetFieldByNumber(theStruct,0,6,boolMat2Matlab(&msg.ack_required,1,1));
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.msg_seq,1,1));
    {
        const char *charString=msg.text.c_str();
        mxSetFieldByNumber(theStruct,0,8,mxCreateCharMatrixFromStrings(1,&charString));
    }
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_1_11_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 236, which was revoked
//in IMO circular 289 in 2013. The note says that if information is not
//available, then a field will be set to the max value. Thus, validity
//below is determined by seeing if data is withing the valid range.
//(Message "METEOROLOGICAL AND HYDROLOGICAL DATA")
    const size_t numberOfFields=43;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi", "x", "y", "day", "hour", "minute",
    "wind_ave", "wind_gust", "wind_dir", "wind_gust_dir", "air_temp",
    "rel_humid", "dew_point", "air_pres", "air_pres_trend", "horz_vis",
    "water_level", "water_level_trend", "surf_cur_speed", "surf_cur_dir",
    "cur_speed_2", "cur_dir_2", "cur_depth_2", "cur_speed_3", "cur_dir_3",
    "cur_depth_3", "wave_height", "wave_period", "wave_dir", 
    "swell_height", "swell_period", "swell_dir", "sea_state", "water_temp",
    "precip_type", "salinity", "ice", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_11 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Longitude East in degrees (WGS-84)",//6
            "Latitude North in degrees (WGS-84)",//7
            "UTC day transmitted (1-31)",//8
            "UTC hour transmitted (0-23)",//9
            "UTC minute transmitted (0-59)",//10
            "Average wind speed for the last 10 minutes in knots (0-120)",//11
            "Maximum wind gust speed during the last 10 minutes in knots (0-120)",//12
            "Wind direction in degrees East of North",//13
            "Wind gust direction in degrees East of North",//14
            "Dry buld air temperature in degrees Celsius (-60 to 60)",//15
            "Relative humidity in percent (0-100)",//16
            "Dew point in degrees Celsius (-20 to 50)",//17
            "Air pressure in hectoPascals (800-1200)",//18
            "Air pressure tendency 0 = steady, 1 = decreasing, 2 = increasing",//19
            "Horizontal visibility in nautical miles (0-25)",//20
            "Water level, including tide, deviation from local datum in meters (-10 to 30)",//21
            "Water level trend 0 = steady, 1 = decreasing, 2 = increasing",//22
            "Surface current speed including tide in knots (0-25)",//23
            "Surface current direction in degrees East of North (0-359)",//24
            "Current 2 speed in knots (0-25)",//25
            "Current 2 speed direction in degrees East of North (0-359)",//26
            "Depth of current 2 in meters (0-30)",//27
            "Current 3 speed in knots (0-25)",//28
            "Current 3 speed direction in degrees East of North (0-359)",//29
            "Depth of current 3 in meters (0-30)",//30
            "Significant wave height in meters (0-25)",//31
            "Wave period in seconds (0-60)",//32
            "Wave direction in degrees East of North (0-359)",//33
            "Swell height in meters (0-25)",//34
            "Swell period in seconds (0-60)",//35
            "Swell direction in degrees East of North (0-359)",//36
            "Sea state according to the Beauford scale (0-12)",//37
            "Water temperature in degrees Celsius (-10 to 50)",//38
            "Precipitation type according to the WMO\n0 = reserved\n1 = rain\n2 = thunderstorm\n3 = freezing rain\n4 = mixed/ice\n5 = snow\n6 = reserved",//39
            "Salinity in parts per thousand (0-50)",//40
            "A boolean value indicating the presence of ice",//41
            "Not used. Should be zero."//42
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais8_1_11 type.
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//No longitude
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//No latitude
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.day!=0&&msg.day<=31) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.day,1,1));
    } else {//No day
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.hour<24) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.hour,1,1));
    } else {//No hour
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.minute<60) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.minute,1,1));
    } else {//No minute
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_ave<=120) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.wind_ave,1,1));
    } else {//No wind average
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_gust<=120) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.wind_gust,1,1));
    } else {//No wind gust speed
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_dir<=359) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.wind_dir,1,1));
    } else {//No wind direction
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_gust_dir<=359) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.wind_gust_dir,1,1));
    } else {//No wind gust direction
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.air_temp<=60&&msg.air_temp>=-60) {
        mxSetFieldByNumber(theStruct,0,15,floatMat2MatlabDoubles(&msg.air_temp,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.rel_humid<=100) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.rel_humid,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.dew_point<=50&&msg.dew_point>=-20) {
        mxSetFieldByNumber(theStruct,0,17,floatMat2MatlabDoubles(&msg.dew_point,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.air_pres<=1200) {
        mxSetFieldByNumber(theStruct,0,18,floatMat2MatlabDoubles(&msg.air_pres,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.air_pres_trend<=2){
        mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.air_pres_trend,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.horz_vis<=25) {
        mxSetFieldByNumber(theStruct,0,20,floatMat2MatlabDoubles(&msg.horz_vis,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.water_level<=30&&msg.water_level>=-10) {
        mxSetFieldByNumber(theStruct,0,21,floatMat2MatlabDoubles(&msg.water_level,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.water_level_trend<=2) {
        mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.water_level_trend,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.surf_cur_speed<=25) {
        mxSetFieldByNumber(theStruct,0,23,floatMat2MatlabDoubles(&msg.surf_cur_speed,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.surf_cur_dir<=359) {
        mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.surf_cur_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_speed_2<=25) {
        mxSetFieldByNumber(theStruct,0,25,floatMat2MatlabDoubles(&msg.cur_speed_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_dir_2<=359) {
        mxSetFieldByNumber(theStruct,0,26,intMat2MatlabDoubles(&msg.cur_dir_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,26,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_depth_2<=30) {
        mxSetFieldByNumber(theStruct,0,27,intMat2MatlabDoubles(&msg.cur_depth_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,27,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_speed_3<=25) {
        mxSetFieldByNumber(theStruct,0,28,floatMat2MatlabDoubles(&msg.cur_speed_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,28,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_dir_3<=359) {
        mxSetFieldByNumber(theStruct,0,29,intMat2MatlabDoubles(&msg.cur_dir_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,29,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_depth_3<=30) {
        mxSetFieldByNumber(theStruct,0,30,intMat2MatlabDoubles(&msg.cur_depth_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,30,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wave_height<=25) {
        mxSetFieldByNumber(theStruct,0,31,floatMat2MatlabDoubles(&msg.wave_height,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,31,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.wave_period<=60) {
        mxSetFieldByNumber(theStruct,0,32,intMat2MatlabDoubles(&msg.wave_period,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,32,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.wave_dir<=359) {
        mxSetFieldByNumber(theStruct,0,33,intMat2MatlabDoubles(&msg.wave_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,33,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.swell_height<=25) {
        mxSetFieldByNumber(theStruct,0,34,floatMat2MatlabDoubles(&msg.swell_height,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,34,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.swell_period<=60) {
        mxSetFieldByNumber(theStruct,0,35,intMat2MatlabDoubles(&msg.swell_period,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,35,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.swell_dir<=359) {
        mxSetFieldByNumber(theStruct,0,36,intMat2MatlabDoubles(&msg.swell_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,36,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.sea_state<=12) {
        mxSetFieldByNumber(theStruct,0,37,intMat2MatlabDoubles(&msg.sea_state,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,37,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.water_temp<=50&&msg.water_temp>=-10) {
        mxSetFieldByNumber(theStruct,0,38,floatMat2MatlabDoubles(&msg.water_temp,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,38,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.precip_type<7) {
        mxSetFieldByNumber(theStruct,0,39,intMat2MatlabDoubles(&msg.precip_type,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,39,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.salinity<511) {
        mxSetFieldByNumber(theStruct,0,40,floatMat2MatlabDoubles(&msg.salinity,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,40,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.ice==0||msg.ice==1) {
        mxSetFieldByNumber(theStruct,0,41,intMat2MatlabDoubles(&msg.ice,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,41,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }

    mxSetFieldByNumber(theStruct,0,42,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_1_13_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 236, which was revoked
//in IMO circular 289 in 2013. Message "FAIRWAY CLOSED".
    const size_t numberOfFields=20;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi", "reason", "location_from", "location_to",
    "radius", "units", "day_from", "month_from", "hour_from",
    "minute_from", "day_to", "month_to", "hour_to", "minute_to", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_13 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
      
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Reason for closing",//6
            "Location of closing from",//7
            "Location of closing to",//8
            "Extension of closed area (radius, 0-1000)",//9
            "Unit of extension value, 0=meters, 1=kilometers, 2=nautical miles, 3=cable length (presumably meaning 0.1 nautical miles)",//10
            "Closing from day (1-31)",//11
            "Closing from month (1-12)",//12
            "From LT hour (0-23) (approx)",//13
            "From LT minute (0-59) (approx)",//14
            "To day (1-31)",//15
            "To month (1-12)",//16
            "To LT hour (0-23) (approx)",//17
            "To LT minute (0-59) (approx)",//18
            "Not used. Should be zero"//19
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais8_1_13 type.
    if(msg.reason.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.reason.c_str();
        mxSetFieldByNumber(theStruct,0,6,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//The reason is unavailable
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.location_from.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.location_from.c_str();
        mxSetFieldByNumber(theStruct,0,7,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//The location of closing from is unavailable
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.location_to.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.location_to.c_str();
        mxSetFieldByNumber(theStruct,0,8,mxCreateCharMatrixFromStrings(1,&charString));
    } else {//The location of closing to is unavailable
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.radius<=1000) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.radius,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.units,1,1));
    if(msg.day_from!=0) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.day_from,1,1));
    } else {//Day unavialable
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.month_from!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.month_from,1,1));
    } else {//Month unavailable
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.hour_from<=23) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.hour_from,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.minute_from<=59) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.minute_from,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.day_to!=0) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.day_to,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.month_to!=0) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.month_to,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.hour_to<=23) {
        mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.hour_to,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.minute_to<=59) {
        mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.minute_to,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_1_15_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 236, which was revoked
//in IMO circular 289 in 2013. Message "EXTENDED SHIP STATIC AND VOYAGE
//RELATED DATA"
    const size_t numberOfFields=8;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi","air_draught", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_15 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Air draught, heigh over keel, in meters (0.1-204.7), 204.7 mean 204.7 meters or higher)",//6
            "Not used. Should be set to zero."//7
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais8_1_15 type.
    if(msg.air_draught!=0) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.air_draught,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_1_16_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circulars 236 and 289.
//Message "Number of persons on board"
    const size_t numberOfFields=8;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi","persons", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_16 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Number of persons currently on board including crew members (1-8190), 8190 means 8190 or greater",//6
            "Not used. Set to zero."//7
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais8_1_16 type.
    if(msg.persons!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.persons,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}


AIS_STATUS AIS_8_1_17_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circulars 236 and 289.
//Message "VTS-generated/Synthetic targets"
    const size_t numberOfFields=7;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi","targets"};
    const size_t numberOfTargetFields=8;
    mwSize targetDims[2];
    const char *targetFieldNames[numberOfTargetFields] = {"type", "id",
    "spare", "x", "y", "cog", "timestamp" "sog"};
    mxArray *theStruct, *targetArray;
    size_t curTarget;
    
    Ais8_1_17 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            ""//6 This is not used. Instead, a strcture describing the target parameters is used.
        };
        const char *targetDescriptions[numberOfTargetFields] = {
            "The type of the target identifier:\n0 = The target identifier is the MMSI number\n1 = The target identifier is the IMO number\n2 = The target identifier is the call sign\n3 = Other (default)",//0
           "Target identifier as a string of characters",//1
           "Spare. Set to zero.",//2
           "Longitude of the target in degrees East (WGS-84)",//3
           "Latitude of the target in degrees North (WGS-84)",//4
           "Course over ground in degrees East of north (0-359)",//5
           "UTC second when the report was generated (0-59)",//6
           "Speed over ground in knots (0-254)"//7
        };
        int i;
        mxArray *targetFields;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
        for(i=0;i<(numberOfFields-1);i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
        
        targetFields=mxCreateStructArray(2, dims, numberOfTargetFields, targetFieldNames);
        mxSetFieldByNumber(*fieldDescriptions,0,numberOfFields-1,targetFields);

        for(i=0;i<numberOfTargetFields;i++) {
            mxSetFieldByNumber(targetFields,0,i,mxCreateCharMatrixFromStrings(1,&targetDescriptions[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais8_1_17 type.
    targetDims[0]=msg.targets.size();
    targetDims[1]=1;
    targetArray=mxCreateStructArray(2, dims, numberOfTargetFields, targetFieldNames);
    mxSetFieldByNumber(theStruct,0,6,targetArray);
    for (curTarget = 0; curTarget < msg.targets.size(); curTarget++) {
        mxSetFieldByNumber(targetArray,curTarget,0,intMat2MatlabDoubles(&msg.targets[curTarget].type,1,1));
        if(msg.targets[curTarget].id.compare("@@@@@@@")!=0) {
            const char *charString=msg.targets[curTarget].id.c_str();
            mxSetFieldByNumber(targetArray,curTarget,1,mxCreateCharMatrixFromStrings(1,&charString));
        } else {//No target ID
            mxSetFieldByNumber(targetArray,curTarget,1,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        mxSetFieldByNumber(targetArray,curTarget,2,intMat2MatlabDoubles(&msg.targets[curTarget].spare,1,1));
        if(fabs(msg.targets[curTarget].x)<=180) {
            mxSetFieldByNumber(targetArray,curTarget,3,floatMat2MatlabDoubles(&msg.targets[curTarget].x,1,1));
        } else {//No longitude
            mxSetFieldByNumber(targetArray,curTarget,3,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(fabs(msg.targets[curTarget].y)<=90) {
            mxSetFieldByNumber(targetArray,curTarget,4,floatMat2MatlabDoubles(&msg.targets[curTarget].y,1,1));
        } else {//No latitude
            mxSetFieldByNumber(targetArray,curTarget,4,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.targets[curTarget].cog<=359) {
            mxSetFieldByNumber(targetArray,curTarget,5,intMat2MatlabDoubles(&msg.targets[curTarget].cog,1,1));
        } else {//No course over ground
            mxSetFieldByNumber(targetArray,curTarget,5,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.targets[curTarget].timestamp<=59) {
            mxSetFieldByNumber(targetArray,curTarget,6,intMat2MatlabDoubles(&msg.targets[curTarget].timestamp,1,1));
        } else {//No timestamp
            mxSetFieldByNumber(targetArray,curTarget,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
        if(msg.targets[curTarget].sog<=254) {
            mxSetFieldByNumber(targetArray,curTarget,7,intMat2MatlabDoubles(&msg.targets[curTarget].sog,1,1));
        } else {//No speed over ground
            mxSetFieldByNumber(targetArray,curTarget,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
        }
    }
    
    return AIS_OK;
}

AIS_STATUS AIS_8_1_19_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Marine Traffic Signal"
    const size_t numberOfFields=16;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi","link_id", "name", "x", "y", "status",
    "signal", "utc_hour_next", "utc_min_next", "next_signal", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_19 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Message Linkage ID: A source specific running number, unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID, source MMSI and message FI uniquely identifies the sent message. (1-1023)",//6
            "Name of Signal Station as defined in ITU-R M. 1371-3, Table 44",//7
            "Longitude of station in degrees East (WGS-84)",//8
            "Latitude of station in degrees North (WGS-84)",//9
            "Status of Signal: 1 = In regular service, 2 = Irregular service, 3 reserved for future use",//10
            "Signal in service message value as defined in Table 8.2 of IMO circular 289",//11
            "UTC hour of next signal shift (0-23)",//12
            "UTC minute of next signal shift (0-59)",//13
            "Expected next signal as defined in Table 8.2 of IMO circular 289"
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fields specific to the Ais8_1_19 type.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.name.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
        const char *charString=msg.name.c_str();
        mxSetFieldByNumber(theStruct,0,7,mxCreateCharMatrixFromStrings(1,&charString));
    } else {
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,9,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.status!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.status,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.signal!=0) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.signal,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_hour_next<=23) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_hour_next,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_min_next<=59) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_min_next,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.next_signal!=0) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.next_signal,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(msg.spare2,4,1));

    return AIS_OK;
}

/*
AIS_STATUS AIS_8_1_21_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Weather observation report from ship"
    const mwSize dims[2] = {1, 1};

    mxArray *theStruct;
    
    Ais8_1_21 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    switch(msg.type_wx_report) {
        case 0:
        {
            const size_t numberOfFields=29;
            //These are the names of all of the fields that will be added to the
            //Matlab structre array.
            const char *fieldNames[numberOfFields] = {"message_id",
            "repeat_indicator", "mmsi","spare","dac", "fi",
            "type_wx_report", "location", "x", "y", "utc_day", "utc_hour",
            "utc_min", "wx","horz_viz", "humidity", "wind_speed", "wind_dir",
            "pressure", "pressure_tendency", "air_temp", "water_temp",
            "wave_period", "wave_height", "wave_dir", "swell_height",
            "swell_dir", "swell_period", "spare2"};
             if(fieldDescriptions!=NULL) {
                const char *descriptionStrings[numberOfFields] = {
                "Identifier for Message 6; always 6.",//0
                "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
                "Maritime mobile service identity (MMSI) number of source station",//2
                "Not used. Should be zero. Reserved for future use",//3
                "Designated area code (DAC)",//4
                "Function identifier (FI)",//5
                "Type of weather report. Here, 1=WMO Weather observation report from ship",//6
                "Geographic location as an ASCII string as defined in ITU-R M. 1371-3, Table 44",//7
                "Longitude of observation in degrees East (WGS-84)",//8
                "Latitude of observation in degrees North (WGS-84)",//9
                "UTC day of observation (1-31)",//10
                "UTC hour of observation (0-24)",//11
                "UTC minute of observation (0-59)",//12
                "Present weather based on WMI Code 45501:\n0 = clear (no clouds at any level)\n1 = cloudy\n2 = rain\n3 = fog\n4 = snow\n5 = typhoon/hurricane\n6 = monsoon\n7 = thunderstorm\n8 = not available (an empty matrix is returned instead)\n9 - 15 (reserved for future use)",//13
                 "Horizontal visibility in nautical miles (0-12.6)",//14 NOTE: The first bit being set makes this an invalid value, and indicates that the equipment was saturated.
                 "Relative humidity in percent (0-100)",//15
                 "Average wind speed in knots (0-126)",//16
                 "Wind direction in degrees East of North",//17
                 "Air pressure in hectoPascals at sea level (799-1201)",//18
                 "Air pressure tendency using WMO FM13 codes for pressure characteristic over the last three hours.",//19
                 "Dry bulb air temperature in degrees Celsius (-60 to 60)",//20
                 "Temperature of the water in degrees Celsius (-10 to 50)",//21
                 "Wave period in seconds",//22
                 "Height of the waves in meters",//23
                 "Wave direction in degrees East of North (0-359)",//24
                 "Swell height in meters",//25
                 "Swell direction in degrees East of North (0-359)",//26
                 "Swell period in seconds",//27
                 "Not used. Set to zero."//28
                };
                int i;

                *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

                for(i=0;i<numberOfFields;i++) {
                    mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
                }
            };

            //Create the Matlab structure array.
            theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
            *matlabStruct=theStruct;
            
            //Fill in the components unique to this message.
            if(msg.location.compare("@@@@@@@@@@@@@@@@@@@@")!=0) {
                const char *charString=msg.location.c_str();
                mxSetFieldByNumber(theStruct,0,7,mxCreateCharMatrixFromStrings(1,&charString));
            } else {
                mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            if(fabs(msg.x)<=180) {
                mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.x,1,1));
            } else {//No longitude
                mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            if(fabs(msg.y)<=90) {
                mxSetFieldByNumber(theStruct,0,9,floatMat2MatlabDoubles(&msg.y,1,1));
            } else {//No latitude
                mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            if(msg.utc_day!=0) {
                mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_day,1,1));
            } else {//No UTC day
                mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            if(msg.utc_hour<=23) {
                mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_hour,1,1));
            } else{//No UTC hour
                mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            if(msg.utc_min<=59) {
                mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_min,1,1));
            } else {//No UTC minutes
                mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.wx[0],1,1));
            if(msg.horz_viz!=12.7) {
                mxSetFieldByNumber(theStruct,0,14,floatMat2MatlabDoubles(&msg.horz_viz,1,1));
            } else {//No Horizontal visibility
                mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            if(msg.humidity<=100) {
                mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.humidity,1,1));
            } else {//No humidity
                mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_speed<=126) {
                mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.wind_speed,1,1));
            } else {//No wind speed
                mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_dir<=359) {
                mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.wind_dir,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.pressure<=1201) {
                mxSetFieldByNumber(theStruct,0,18,floatMat2MatlabDoubles(&msg.pressure,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.pressure_tendency,1,1));
            if(msg.air_temp>=-60&&msg.air_temp<=60) {
                mxSetFieldByNumber(theStruct,0,20,floatMat2MatlabDoubles(&msg.air_temp,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.water_temp>=-10&&msg.water_temp<=50) {
                mxSetFieldByNumber(theStruct,0,21,floatMat2MatlabDoubles(&msg.water_temp,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wave_period<=60) {
                mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.wave_period,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wave_height<=25.1) {
                mxSetFieldByNumber(theStruct,0,23,floatMat2MatlabDoubles(&msg.wave_height,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wave_dir<=359) {
                mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.wave_dir,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_height<=25.1) {
                mxSetFieldByNumber(theStruct,0,25,floatMat2MatlabDoubles(&msg.swell_height,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_dir<=359) {
                mxSetFieldByNumber(theStruct,0,26,intMat2MatlabDoubles(&msg.swell_dir,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,26,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_period<=60) {
                mxSetFieldByNumber(theStruct,0,27,intMat2MatlabDoubles(&msg.swell_period,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,27,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            mxSetFieldByNumber(theStruct,0,28,intMat2MatlabDoubles(&msg.spare2,1,1));
        }
            break;
        case 1:
        {
            const size_t numberOfFields=54;
            //These are the names of all of the fields that will be added to the
            //Matlab structre array.
            const char *fieldNames[numberOfFields] = {"message_id",
            "repeat_indicator", "mmsi", "spare", "dac", "fi",
            "type_wx_report", "x", "y",
            "utc_month", "utc_day", "utc_hour", "utc_min", "cog", "sog",
            "heading", "pressure", "rel_pressure", "pressure_tendency",
            "wind_dir", "wind_speed_ms", "wind_dir_rel", "wind_speed_rel",
            "wind_gust_speed", "wind_gust_dir", "air_temp_raw", "humidity",
            "water_temp_raw", "horz_viz", "present_weather",
            "past_weather_1", "past_weather_2",
            "cloud_total", "cloud_low", "cloud_low_type",
            "cloud_middle_type", "cloud_high_type",
            "alt_lowest_cloud_base", "wave_period", "wave_height",
            "swell_dir", "swell_period", "swell_height", "swell_dir_2",
            "swell_period_2", "swell_height_2", "ice_thickness",
            "ice_accretion", "ice_accretion_cause",
            "sea_ice_concentration", "amt_type_ice", "ice_situation",
            "ice_devel", "bearing_ice_edge"};
            if(fieldDescriptions!=NULL) {
                const char *descriptionStrings[numberOfFields] = {
                "Identifier for Message 6; always 6.",//0
                "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
                "Maritime mobile service identity (MMSI) number of source station",//2
                "Not used. Should be zero. Reserved for future use",//3
                "Designated area code (DAC)",//4
                "Function identifier (FI)",//5
                "Type of weather report. Here, 0=WMO weather observation report from ship",//6
                "Longitude of observation in degrees East (WGS-84)",//7
                "Latitude of observation in degrees North (WGS-84)",//8
                "UTC month of observation (1-12)",//9
                "UTC day of observation (1-31)",//10
                "UTC hour of observation (0-24)",//11
                "UTC minute of observation (0-59)",//12
                "Ship course over ground for the past 10 minutes in degrees East of North (5-360). 0 degrees means stopped",//13
                "Ship speed over ground for the past 10 minutes in meters per second (0-15)",//14
                "Average heading of the ship for the past 10 minutes in degrees East of north (5-360). 0 degrees means stopped",//15
                "Air pressure in hectoPascals reduced to sea level (900-1100)",//16
                "3-hour air pressure change in hectoPascals (-50 to 50)",//17
                "Characteristic of pressure tendency from WMO BUFR table 010063",//18
                "True wind direction averaged over 10 minutes in degrees East of North (5-360). 0 means calm.",//19
                "True wind speed averaged over 10 minutes in meters per second (0-127)",//20
                "Relative wind direction averaged over 10 minutes in degrees East of North (5-360). 0 degrees means calm",//21
                "Relative wind speed averaged over 10 minutes in meters per second (0-127)",//22
                "Maximum wind gust speed in meters per second (0-127)",//23
                "Maximum wind gust direction in degrees East of North. 0 degrees means calm",//24
                "Dry bulb air temperature such that the temperature in degrees Kelvin=air_temp_raw/10+223",//25
                "Relative humidity in percent (0-100)",//26
                "Sea surface temperature such that the temperature in degrees Kelvin=water_temp_raw/10+268",//27
                "Horizontal visibility in meters (0-50000m)",//28
                "Present weather as given by codes 0-510 in WMO BUFR table 020003",//29
                "Past weather 1 as given by codes 0-30 in WMO BUFR table 020004",//30
                "Past weather 2 as given by codes 0-30 in WMO BUFR table 020005",//31
                "Total cloud cover in percent (0-100)",//32
                "Cloud amount low as given by WMO BUFR table 020011",//33
                "Cloud type low as given by WMO BUFR table 020012",//34
                "Cloud type middle as given by WMO BUFR table 020012",//35
                "Cloud type high as given by WMO BUFR table 020012",//36
                "Height of base of lowest cloud in meters (0-2540.16)",//37
                "Period of wind waves in seconds (0-30)",//38
                "Height of wind waves in meters (0-30)",//39
                "Direction from which the first swell is coming in degrees East of North (10-360). 0 means calm",//40
                "Period of the first swell in seconds (0-30)",//41
                "Height of the first swell in meters (0-30)",//42
                "Direction from which the second swell is coming in degrees East of North (10-360). 0 means calm",//43
                "Period of the second swell in seconds (0-30)",//44
                "Height of the second swell in meters (0-30)",//45
                "Ice deposit thickness in centimeters",//46
                "Rate of ice accretion as per WMO BUFR table 020032",//47
                "Cause of ice accretion as per WMO BUFR table 020033",//48
                "Sea ice concentration as per WMO BUFR table 020034",//49
                "Amount and type of ice as per WMO BUFR table 020035",//50
                "Ice situation as per WMO BUFR table 020035",//51
                "Ice development as per WMO BUFR table 020037",//52
                "Bearing of ice edge in degrees East of North (45-360)."//53
                };
                int i;

                *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

                for(i=0;i<numberOfFields;i++) {
                    mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
                }
             };
            
            //Create the Matlab structure array.
            theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
            *matlabStruct=theStruct;
            
            if(fabs(msg.x)<=180) {
                mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.x,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(fabs(msg.y)<=90) {
                mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.y,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.utc_month!=0) {
                mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.utc_month,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.utc_day!=0) {
                mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_day,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.utc_hour<=23) {
                mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_hour,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.utc_min<=59) {
                mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_min,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.cog<=360) {
                mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.cog,1,1));
            } else { 
                mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.sog<=15) {
                mxSetFieldByNumber(theStruct,0,14,floatMat2MatlabDoubles(&msg.sog,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.heading<=360){
                mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.heading,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.pressure<=1100) {
                mxSetFieldByNumber(theStruct,0,16,floatMat2MatlabDoubles(&msg.pressure,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.rel_pressure<=50&&msg.rel_pressure<=50) {
                mxSetFieldByNumber(theStruct,0,17,floatMat2MatlabDoubles(&msg.rel_pressure,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
            }

            mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.pressure_tendency,1,1));
            if(msg.wind_dir<=360) {
                mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.wind_dir,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_speed_ms<=127){
                mxSetFieldByNumber(theStruct,0,20,floatMat2MatlabDoubles(&msg.wind_speed_ms,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_dir_rel<=360) {
                mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.wind_dir_rel,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_speed_rel<=127) {
                mxSetFieldByNumber(theStruct,0,22,floatMat2MatlabDoubles(&msg.wind_speed_rel,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_gust_speed<=127) {
                mxSetFieldByNumber(theStruct,0,23,floatMat2MatlabDoubles(&msg.wind_gust_speed,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wind_gust_dir<=360) {
                mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.wind_gust_dir,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.air_temp_raw!=1023) {
                mxSetFieldByNumber(theStruct,0,25,intMat2MatlabDoubles(&msg.air_temp_raw,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.humidity<=100) {
                mxSetFieldByNumber(theStruct,0,26,intMat2MatlabDoubles(&msg.humidity,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,26,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.water_temp_raw!=511) {
                mxSetFieldByNumber(theStruct,0,27,intMat2MatlabDoubles(&msg.water_temp_raw,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,27,mxCreateDoubleMatrix(0,0,mxREAL));
            } 
            if(msg.horz_viz<=50000) {
                mxSetFieldByNumber(theStruct,0,28,floatMat2MatlabDoubles(&msg.horz_viz,1,1));
            } else {//No Horizontal visibility
                mxSetFieldByNumber(theStruct,0,28,mxCreateDoubleMatrix(0,0,mxREAL)); 
            }
            mxSetFieldByNumber(theStruct,0,29,intMat2MatlabDoubles(&msg.wx[0],3,1));
            mxSetFieldByNumber(theStruct,0,30,intMat2MatlabDoubles(&msg.wx[1],3,1));
            mxSetFieldByNumber(theStruct,0,31,intMat2MatlabDoubles(&msg.wx[2],3,1));
            
            if(msg.cloud_total<=100) {
                mxSetFieldByNumber(theStruct,0,32,intMat2MatlabDoubles(&msg.cloud_total,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,32,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.cloud_low!=15) {
                mxSetFieldByNumber(theStruct,0,33,intMat2MatlabDoubles(&msg.cloud_low,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,33,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.cloud_low_type!=63) {
                mxSetFieldByNumber(theStruct,0,34,intMat2MatlabDoubles(&msg.cloud_low_type,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,34,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.cloud_middle_type!=63) {
                mxSetFieldByNumber(theStruct,0,35,intMat2MatlabDoubles(&msg.cloud_middle_type,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,35,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.cloud_high_type!=63) {
                mxSetFieldByNumber(theStruct,0,36,intMat2MatlabDoubles(&msg.cloud_high_type,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,36,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.alt_lowest_cloud_base<=2540.16) {
                mxSetFieldByNumber(theStruct,0,37,floatMat2MatlabDoubles(&msg.alt_lowest_cloud_base,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,37,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wave_period<=30) {
                mxSetFieldByNumber(theStruct,0,38,intMat2MatlabDoubles(&msg.wave_period,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,38,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.wave_height<=30) {
                mxSetFieldByNumber(theStruct,0,39,floatMat2MatlabDoubles(&msg.wave_height,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,39,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_dir<=360) {
                mxSetFieldByNumber(theStruct,0,40,intMat2MatlabDoubles(&msg.swell_dir,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,40,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_period<=30) {
                mxSetFieldByNumber(theStruct,0,41,intMat2MatlabDoubles(&msg.swell_period,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,41,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_height<=30) {
                mxSetFieldByNumber(theStruct,0,42,floatMat2MatlabDoubles(&msg.swell_height,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,42,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_dir_2<=360) {
                mxSetFieldByNumber(theStruct,0,43,intMat2MatlabDoubles(&msg.swell_dir_2,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,43,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_period_2<=30) {
                mxSetFieldByNumber(theStruct,0,44,intMat2MatlabDoubles(&msg.swell_period_2,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,44,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.swell_height_2<=30) {
                mxSetFieldByNumber(theStruct,0,45,intMat2MatlabDoubles(&msg.swell_height_2,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,45,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.ice_thickness<=126) {
                mxSetFieldByNumber(theStruct,0,46,floatMat2MatlabDoubles(&msg.ice_thickness,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,46,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.ice_accretion!=7) {
                mxSetFieldByNumber(theStruct,0,47,intMat2MatlabDoubles(&msg.ice_accretion,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,47,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.ice_accretion_cause!=7) {
                mxSetFieldByNumber(theStruct,0,48,intMat2MatlabDoubles(&msg.ice_accretion_cause,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,48,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.sea_ice_concentration!=31) {
                mxSetFieldByNumber(theStruct,0,49,intMat2MatlabDoubles(&msg.sea_ice_concentration,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,49,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.amt_type_ice!=15) {
                mxSetFieldByNumber(theStruct,0,50,intMat2MatlabDoubles(&msg.amt_type_ice,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,50,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.ice_situation!=31) {
                mxSetFieldByNumber(theStruct,0,51,intMat2MatlabDoubles(&msg.ice_situation,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,51,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.ice_devel!=31) {
                mxSetFieldByNumber(theStruct,0,52,intMat2MatlabDoubles(&msg.ice_devel,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,52,mxCreateDoubleMatrix(0,0,mxREAL));
            }
            if(msg.bearing_ice_edge!=15) {
                mxSetFieldByNumber(theStruct,0,53,intMat2MatlabDoubles(&msg.bearing_ice_edge,1,1));
            } else {
                mxSetFieldByNumber(theStruct,0,53,mxCreateDoubleMatrix(0,0,mxREAL));
            }
        }
            break;
        default://If the specific type could not be identified, just
        {
            const size_t numberOfFields=7;
            //These are the names of all of the fields that will be added to the
            //Matlab structre array.
            const char *fieldNames[numberOfFields] = {"message_id",
            "repeat_indicator", "mmsi","spare","dac","fi",
            "type_wx_report"};
            //Return the elements common to all types.
            //Create the Matlab structure array.
            if(fieldDescriptions!=NULL) {
                const char *descriptionStrings[numberOfFields] = {
                "Identifier for Message 6; always 6.",//0
                "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
                "Maritime mobile service identity (MMSI) number of source station",//2
                "Not used. Should be zero. Reserved for future use",//3
                "Designated area code (DAC)",//4
                "Function identifier (FI)",//5
                "Type of ship weather report"//6
                };
                int i;

                *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

                for(i=0;i<numberOfFields;i++) {
                    mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
                }
            }
            
            theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
            *matlabStruct=theStruct;
        }
            break;
    }
        
    //Fill in the first 4 field,s which are common to all of the types. 
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.type_wx_report,1,1));
    
    return AIS_OK;
}
*/

AIS_STATUS AIS_8_1_24_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Extended ship static and voyage-related data (broadcast)"
    const size_t numberOfFields=23;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "link_id",
    "air_draught", "last_port", "next_ports", "solas_status", "ice_class",
    "shaft_power", "vhf", "lloyds_ship_type", "gross_tonnage",
    "laden_ballast", "heavy_oil", "light_oil", "diesel", "bunker_oil",
    "persons", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_24 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "A source specific running number (1-1023), unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID and the first six digits of the source MMSI uniquely identify the sent message.",//6
            "The vertical distance from the waterline to the highest point on the ship in meters. (1-81.9m)",//7
            "UN LOCODE of last port of call",//8
            "UN LOCODEs of next two ports of call",//9
            "SOLAS Equipment status, see IMO circular 289",//10
            "Ice class:\n0 = not classified\n1 = IACS PC 1\n2 = IACS PC 2\n3 = IACS PC 3\n4 = IACS PC 4\n5 = IACS PC 5\n6 = IACS PC 6 / FSICR IA Super / RS Arc5\n7 = IACS PC 7 / FSICR IA / RS Arc4\n8 = FSICR IB / RS Ice3\n9 = FSICR IC / RS Ice2\n10 = RS Ice1\n11 - 14 (reserved for future use)",//11
             "Shaft horse power (0-162142)",//12
             "VHF working channel number as per  ITU-R M.1084",//13
             "Lloyd's ship type, a string",//14
             "Gross tonnage (a measure of size) (0-262142) as per the International Convention on Tonnage Measurement of Ships, 1969",//15
             "1=Laden, 2=Ballast, 3=Not in use",//16
             "Heavy Fuel Oil:\n1 = no\n2 = yes\n3 = not in use",//17
             "Light Fuel Oil:\n1 = no\n2 = yes\n3 = not in use",//18
             "Diesel:\n1 = no\n2 = yes\n3 = not in use",//19
             "Total amount of bunker oil in tonnes",//20
             "Number of persons currently ob board including crew members (1-8191)",//21
             "Not used. Set to zero."//22
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.link_id,1,1));
    mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.air_draught,1,1));
    if(msg.last_port.compare("@@@@@")!=0) {
        const char *charString=msg.last_port.c_str();
        mxSetFieldByNumber(theStruct,0,8,mxCreateCharMatrixFromStrings(1,&charString));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    {
        const char *theStrings[2]={msg.next_ports[0].c_str(),msg.next_ports[1].c_str()};

        if(msg.next_ports[0].compare("@@@@@")!=0&&msg.next_ports[1].compare("@@@@@")!=0) {
            mxSetFieldByNumber(theStruct,0,9,mxCreateCharMatrixFromStrings(2,theStrings));
        } else if (msg.next_ports[0].compare("@@@@@")!=0&&msg.next_ports[1].compare("@@@@@")==0) {
            mxSetFieldByNumber(theStruct,0,9,mxCreateCharMatrixFromStrings(1,theStrings));
        } else if (msg.next_ports[0].compare("@@@@@")==0&&msg.next_ports[1].compare("@@@@@")!=0) {
            mxSetFieldByNumber(theStruct,0,9,mxCreateCharMatrixFromStrings(1,&theStrings[1]));
        } else {
            mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    }
    
    mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(msg.solas_status,26,1));

    if(msg.ice_class!=15) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.ice_class,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    
    if(msg.shaft_power!=262143) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.shaft_power,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.vhf!=0) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.vhf,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.lloyds_ship_type.compare("@@@@@@@")!=0) {
        const char *charString=msg.lloyds_ship_type.c_str();
        mxSetFieldByNumber(theStruct,0,14,mxCreateCharMatrixFromStrings(1,&charString));
    } else {
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.gross_tonnage!=262143){
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.gross_tonnage,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.laden_ballast!=0) {
        mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.laden_ballast,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.heavy_oil!=0) {
        mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.heavy_oil,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.light_oil!=0) {
        mxSetFieldByNumber(theStruct,0,18,intMat2MatlabDoubles(&msg.light_oil,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.diesel!=0) {
        mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.diesel,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.bunker_oil!=16383) {
        mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.bunker_oil,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.persons!=0) {
        mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.persons,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_1_26_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Environmental"
    const size_t numberOfFields=7;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "reports"};
    mwSize cellArrayDims[2];
    mxArray *theStruct, *reportsCellArray;
    size_t curReport;
    
    Ais8_1_26 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "A cell array containing sensor reports as structures where the structures fields are as given in IMO circular 289"//6
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    cellArrayDims[0]=msg.reports.size();
    cellArrayDims[1]=1;
    //Make space to hold all of the reports in a cell array.
    reportsCellArray=mxCreateCellArray(2, cellArrayDims);
    mxSetFieldByNumber(theStruct,0,6,reportsCellArray);

    //Fill in all of the reports.
    for(curReport=0;curReport<msg.reports.size();curReport++) {
        switch (msg.reports[curReport]->report_type) { 
            case AIS8_1_26_SENSOR_LOCATION:
            {
                const size_t numReportFields=11;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "x", "y", "z",
                "owner", "timeout", "spare"};                      
                Ais8_1_26_Location *rpt = reinterpret_cast<Ais8_1_26_Location *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(fabs(rpt->x)<=180) {
                    mxSetFieldByNumber(reportStruct,0,5,floatMat2MatlabDoubles(&rpt->x,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(fabs(rpt->y)<=90) {
                    mxSetFieldByNumber(reportStruct,0,6,floatMat2MatlabDoubles(&rpt->y,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->z<=200) {
                    mxSetFieldByNumber(reportStruct,0,7,floatMat2MatlabDoubles(&rpt->z,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->owner!=0) {
                    mxSetFieldByNumber(reportStruct,0,8,intMat2MatlabDoubles(&rpt->owner,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,9,intMat2MatlabDoubles(&rpt->timeout,1,1));
                mxSetFieldByNumber(reportStruct,0,10,intMat2MatlabDoubles(&rpt->spare,1,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
                
            }
                break;
            case AIS8_1_26_SENSOR_STATION:
            {
                const size_t numReportFields=7;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "name", "spare"};                      
                Ais8_1_26_Station *rpt = reinterpret_cast<Ais8_1_26_Station *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(rpt->name.compare("@@@@@@@@@@@@@@")!=0) {
                    const char *charString=rpt->name.c_str();
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateCharMatrixFromStrings(1,&charString));
                } else {
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&rpt->spare,1,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_WIND:
            {
                const size_t numReportFields=18;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "wind_speed",
                "wind_gust", "wind_dir", "wind_gust_dir", "sensor_type",
                "wind_forecast", "wind_gust_forecast", "wind_dir_forecast",
                "utc_day_forecast", "utc_hour_forecast",
                "utc_min_forecast", "duration", "spare"};                      
                Ais8_1_26_Wind *rpt = reinterpret_cast<Ais8_1_26_Wind *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);

                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(rpt->wind_speed<=121) {
                    mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&rpt->wind_speed,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wind_gust<=121) {
                    mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&rpt->wind_gust,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wind_dir<=359) {
                    mxSetFieldByNumber(reportStruct,0,7,intMat2MatlabDoubles(&rpt->wind_dir,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wind_gust_dir<=359) {
                    mxSetFieldByNumber(reportStruct,0,8,intMat2MatlabDoubles(&rpt->wind_gust_dir,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->sensor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,9,intMat2MatlabDoubles(&rpt->sensor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wind_forcast<=121) {
                    mxSetFieldByNumber(reportStruct,0,10,intMat2MatlabDoubles(&rpt->wind_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wind_gust_forcast<=121) {
                    mxSetFieldByNumber(reportStruct,0,11,intMat2MatlabDoubles(&rpt->wind_gust_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wind_dir_forcast<=359) {
                    mxSetFieldByNumber(reportStruct,0,12,intMat2MatlabDoubles(&rpt->wind_dir_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_day_forcast!=0) {
                    mxSetFieldByNumber(reportStruct,0,13,intMat2MatlabDoubles(&rpt->utc_day_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hour_forcast<=23) {
                    mxSetFieldByNumber(reportStruct,0,14,intMat2MatlabDoubles(&rpt->utc_hour_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min_forcast<=59) {
                    mxSetFieldByNumber(reportStruct,0,15,intMat2MatlabDoubles(&rpt->utc_min_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,16,intMat2MatlabDoubles(&rpt->duration,1,1));
                mxSetFieldByNumber(reportStruct,0,17,intMat2MatlabDoubles(&rpt->spare,1,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_WATER_LEVEL:
            {
                const size_t numReportFields=17;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "type", "level",
                "trend", "vdatum", "sensor_type", "forecast_type",
                "level_forcast", "utc_day_forcast", "utc_hour_forcast",
                "utc_min_forcast", "duration", "spare"};                      
                Ais8_1_26_WaterLevel *rpt = reinterpret_cast<Ais8_1_26_WaterLevel *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&rpt->type,1,1));
                if(rpt->level>-327.68) {
                    mxSetFieldByNumber(reportStruct,0,6,floatMat2MatlabDoubles(&rpt->level,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->trend!=3) {
                    mxSetFieldByNumber(reportStruct,0,7,intMat2MatlabDoubles(&rpt->trend,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->vdatum!=14) {
                    mxSetFieldByNumber(reportStruct,0,8,intMat2MatlabDoubles(&rpt->vdatum,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,9,intMat2MatlabDoubles(&rpt->sensor_type,1,1));
                mxSetFieldByNumber(reportStruct,0,10,intMat2MatlabDoubles(&rpt->forcast_type,1,1));
                if(rpt->level_forcast>-327.68) {
                    mxSetFieldByNumber(reportStruct,0,11,floatMat2MatlabDoubles(&rpt->level_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_day_forcast!=0) {
                    mxSetFieldByNumber(reportStruct,0,12,intMat2MatlabDoubles(&rpt->utc_day_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hour_forcast<=23) {
                    mxSetFieldByNumber(reportStruct,0,13,intMat2MatlabDoubles(&rpt->utc_hour_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min_forcast<=59) {
                    mxSetFieldByNumber(reportStruct,0,14,intMat2MatlabDoubles(&rpt->utc_min_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,15,intMat2MatlabDoubles(&rpt->duration,1,1));
                mxSetFieldByNumber(reportStruct,0,16,intMat2MatlabDoubles(&rpt->spare,1,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_CURR_2D:
            {
                const size_t numReportFields=8;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "type", "spare",
                "currents"};                      
                Ais8_1_26_Curr2D *rpt = reinterpret_cast<Ais8_1_26_Curr2D *>(msg.reports[curReport]);
                const mwSize currentsDims[2] = {3,1};
                const size_t numberOfCurrentFields=3;
                const char *currentFieldNames[numberOfCurrentFields]={"speed",
                "dir", "depth"};  
                mxArray *reportStruct, *currentsArray;
                size_t curCurrent;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&rpt->type,1,1));
                mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&rpt->spare,1,1));

                currentsArray=mxCreateStructArray(2, currentsDims, numberOfCurrentFields, currentFieldNames);
                mxSetFieldByNumber(reportStruct,0,7,currentsArray);
                for(curCurrent=0;curCurrent<3;curCurrent++) {
                    if(rpt->currents[curCurrent].speed<=246) {
                        mxSetFieldByNumber(currentsArray,curCurrent,0,floatMat2MatlabDoubles(&rpt->currents[curCurrent].speed,1,1));
                    } else {
                        mxSetFieldByNumber(currentsArray,curCurrent,0,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curCurrent].dir<=359) {
                        mxSetFieldByNumber(currentsArray,curCurrent,1,intMat2MatlabDoubles(&rpt->currents[curCurrent].dir,1,1));
                    } else {
                        mxSetFieldByNumber(currentsArray,curCurrent,1,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curCurrent].depth<=362) {
                        mxSetFieldByNumber(currentsArray,curCurrent,2,intMat2MatlabDoubles(&rpt->currents[curCurrent].depth,1,1));
                    } else {
                        mxSetFieldByNumber(currentsArray,curCurrent,2,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                }
                
                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_CURR_3D:
            {
                const size_t numReportFields=8;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "type", "spare",
                "currents"};                      
                Ais8_1_26_Curr3D *rpt = reinterpret_cast<Ais8_1_26_Curr3D *>(msg.reports[curReport]);
                const mwSize currentsDims[2] = {2,1};
                const size_t numberOfCurrentFields=4;
                const char *currentFieldNames[numberOfCurrentFields]={"north",
                "east", "up", "depth"};  
                mxArray *reportStruct, *currentsArray;
                size_t curCurrent;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&rpt->type,1,1));
                mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&rpt->spare,1,1));
                
                currentsArray=mxCreateStructArray(2, currentsDims, numberOfCurrentFields, currentFieldNames);
                mxSetFieldByNumber(reportStruct,0,7,currentsArray);
                for(curCurrent=0;curCurrent<2;curCurrent++) {
                    if(rpt->currents[curCurrent].north<=24.6) {
                        mxSetFieldByNumber(currentsArray,curCurrent,0,floatMat2MatlabDoubles(&rpt->currents[curCurrent].north,1,1));
                    } else {
                        mxSetFieldByNumber(currentsArray,curCurrent,0,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curCurrent].east<=24.6) {
                        mxSetFieldByNumber(currentsArray,curCurrent,1,floatMat2MatlabDoubles(&rpt->currents[curCurrent].east,1,1));
                    } else {
                        mxSetFieldByNumber(currentsArray,curCurrent,1,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curCurrent].up<=24.6) {
                        mxSetFieldByNumber(currentsArray,curCurrent,2,floatMat2MatlabDoubles(&rpt->currents[curCurrent].up,1,1));
                    } else {
                        mxSetFieldByNumber(currentsArray,curCurrent,2,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    mxSetFieldByNumber(currentsArray,curCurrent,3,intMat2MatlabDoubles(&rpt->currents[curCurrent].depth,1,1));
                }
                
                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_HORZ_FLOW:
            {
                const size_t numReportFields=7;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "spare",
                "currents"};                      
                Ais8_1_26_HorzFlow *rpt = reinterpret_cast<Ais8_1_26_HorzFlow *>(msg.reports[curReport]);
                const mwSize flowDims[2] = {2,1};
                const size_t numberOfFlowFields=5;
                const char *flowFieldNames[numberOfFlowFields]={"bearing",
                "dist", "speed", "dir", "level"};  
                mxArray *reportStruct, *flowArray;
                size_t curFlow;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&rpt->spare,1,1));
                
                flowArray=mxCreateStructArray(2, flowDims, numberOfFlowFields, flowFieldNames);
                mxSetFieldByNumber(reportStruct,0,6,flowArray);
                for(curFlow=0;curFlow<2;curFlow++) {
                    if(rpt->currents[curFlow].bearing!=360) {
                        mxSetFieldByNumber(flowArray,curFlow,0,intMat2MatlabDoubles(&rpt->currents[curFlow].bearing,1,1));
                    } else {
                        mxSetFieldByNumber(flowArray,curFlow,0,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curFlow].dist<=121) {
                        mxSetFieldByNumber(flowArray,curFlow,1,intMat2MatlabDoubles(&rpt->currents[curFlow].dist,1,1));
                    } else {
                        mxSetFieldByNumber(flowArray,curFlow,1,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curFlow].speed<=24.6) {
                        mxSetFieldByNumber(flowArray,curFlow,2,floatMat2MatlabDoubles(&rpt->currents[curFlow].speed,1,1));
                    } else {
                        mxSetFieldByNumber(flowArray,curFlow,2,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curFlow].dir<=359) {
                        mxSetFieldByNumber(flowArray,curFlow,3,intMat2MatlabDoubles(&rpt->currents[curFlow].dir,1,1));
                    } else {
                        mxSetFieldByNumber(flowArray,curFlow,3,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                    if(rpt->currents[curFlow].level<=361) {
                        mxSetFieldByNumber(flowArray,curFlow,4,intMat2MatlabDoubles(&rpt->currents[curFlow].level,1,1));
                    } else {
                        mxSetFieldByNumber(flowArray,curFlow,4,mxCreateDoubleMatrix(0,0,mxREAL));
                    }
                }
                
                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_SEA_STATE:
            {
                const size_t numReportFields=18;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "swell_height",
                "swell_period", "swell_dir", "sea_state", "swell_sensor_type",
                "water_temp", "water_temp_depth", "water_sensor_type",
                "wave_height", "wave_period", "wave_dir", "wave_sensor_type",
                "salinity"};                      
                Ais8_1_26_SeaState *rpt = reinterpret_cast<Ais8_1_26_SeaState *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(rpt->swell_height<=24.6) {
                    mxSetFieldByNumber(reportStruct,0,5,floatMat2MatlabDoubles(&rpt->swell_height,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->swell_period<=60) {
                    mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&rpt->swell_period,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->swell_dir<=359) {
                    mxSetFieldByNumber(reportStruct,0,7,intMat2MatlabDoubles(&rpt->swell_dir,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->sea_state<=12) {
                    mxSetFieldByNumber(reportStruct,0,8,intMat2MatlabDoubles(&rpt->sea_state,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->swell_sensor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,9,intMat2MatlabDoubles(&rpt->swell_sensor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->water_temp>=-10&&rpt->water_temp<=50) {
                    mxSetFieldByNumber(reportStruct,0,10,floatMat2MatlabDoubles(&rpt->water_temp,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->water_temp_depth<=12.1) {
                    mxSetFieldByNumber(reportStruct,0,11,floatMat2MatlabDoubles(&rpt->water_temp_depth,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->water_sensor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,12,intMat2MatlabDoubles(&rpt->water_sensor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wave_height<=24.6) {
                    mxSetFieldByNumber(reportStruct,0,13,floatMat2MatlabDoubles(&rpt->wave_height,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wave_period<=60) {
                    mxSetFieldByNumber(reportStruct,0,14,intMat2MatlabDoubles(&rpt->wave_period,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wave_dir<=359) {
                    mxSetFieldByNumber(reportStruct,0,15,intMat2MatlabDoubles(&rpt->wave_dir,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->wave_sensor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,16,intMat2MatlabDoubles(&rpt->wave_sensor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->salinity<=50.1) {
                    mxSetFieldByNumber(reportStruct,0,17,floatMat2MatlabDoubles(&rpt->salinity,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
                }

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_SALINITY:
            {
                const size_t numReportFields=12;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "water_temp",
                "conductivity", "pressure", "salinity", "salinity_type",
                "sensor_type", "spare"};                      
                Ais8_1_26_Salinity *rpt = reinterpret_cast<Ais8_1_26_Salinity *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(rpt->water_temp<=50&&rpt->water_temp>=-10) {
                    mxSetFieldByNumber(reportStruct,0,5,floatMat2MatlabDoubles(&rpt->water_temp,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->conductivity<=7) {
                    mxSetFieldByNumber(reportStruct,0,6,floatMat2MatlabDoubles(&rpt->conductivity,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->pressure<=6000.1) {
                    mxSetFieldByNumber(reportStruct,0,7,floatMat2MatlabDoubles(&rpt->pressure,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->salinity<=50.1) {
                    mxSetFieldByNumber(reportStruct,0,8,floatMat2MatlabDoubles(&rpt->salinity,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,9,intMat2MatlabDoubles(&rpt->salinity_type,1,1));
                if(rpt->sensor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,10,intMat2MatlabDoubles(&rpt->sensor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,11,intMat2MatlabDoubles(rpt->spare,2,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_WX:
            {
                const size_t numReportFields=16;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "air_temp",
                "air_temp_sensor_type", "precip", "horz_vis", "dew_point",
                "dew_point_type", "air_pressure", "air_pressure_trend",
                "air_pressor_type", "salinity", "spare"};                      
                Ais8_1_26_Wx *rpt = reinterpret_cast<Ais8_1_26_Wx *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(rpt->air_temp>=-60&&rpt->air_temp<=60) {
                    mxSetFieldByNumber(reportStruct,0,5,floatMat2MatlabDoubles(&rpt->air_temp,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,5,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->air_temp_sensor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&rpt->air_temp_sensor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,7,intMat2MatlabDoubles(&rpt->precip,1,1));
                if(rpt->horz_vis<=24.1) {
                    mxSetFieldByNumber(reportStruct,0,8,floatMat2MatlabDoubles(&rpt->horz_vis,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->dew_point<=50&&rpt->dew_point>=-20) {
                    mxSetFieldByNumber(reportStruct,0,9,floatMat2MatlabDoubles(&rpt->dew_point,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->dew_point_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,10,intMat2MatlabDoubles(&rpt->dew_point_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->air_pressure<=1201) {
                    mxSetFieldByNumber(reportStruct,0,11,intMat2MatlabDoubles(&rpt->air_pressure,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->air_pressure_trend!=3) {
                    mxSetFieldByNumber(reportStruct,0,12,intMat2MatlabDoubles(&rpt->air_pressure_trend,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->air_pressor_type!=0) {
                    mxSetFieldByNumber(reportStruct,0,13,intMat2MatlabDoubles(&rpt->air_pressor_type,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->salinity<=50.1) {
                    mxSetFieldByNumber(reportStruct,0,14,floatMat2MatlabDoubles(&rpt->salinity,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,15,intMat2MatlabDoubles(&rpt->spare,1,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            case AIS8_1_26_SENSOR_AIR_DRAUGHT:
            {
                const size_t numReportFields=13;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id", "draught", "gap",
                "forcast_gap", "trend", "utc_day_forcast", "utc_hour_forcast",
                "utc_min_forcast", "spare"};                      
                Ais8_1_26_AirDraught *rpt = reinterpret_cast<Ais8_1_26_AirDraught *>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);

                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                //Fill in the components for this sensor report type.
                if(rpt->draught!=0) {
                    mxSetFieldByNumber(reportStruct,0,5,floatMat2MatlabDoubles(&rpt->draught,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->gap!=0) {
                    mxSetFieldByNumber(reportStruct,0,6,floatMat2MatlabDoubles(&rpt->gap,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->forcast_gap!=3) {
                    mxSetFieldByNumber(reportStruct,0,7,floatMat2MatlabDoubles(&rpt->forcast_gap,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->trend!=3) {
                    mxSetFieldByNumber(reportStruct,0,8,intMat2MatlabDoubles(&rpt->trend,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_day_forcast!=0) {
                    mxSetFieldByNumber(reportStruct,0,9,intMat2MatlabDoubles(&rpt->utc_day_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hour_forcast<=23) {
                    mxSetFieldByNumber(reportStruct,0,10,intMat2MatlabDoubles(&rpt->utc_hour_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min_forcast<=59) {
                    mxSetFieldByNumber(reportStruct,0,11,intMat2MatlabDoubles(&rpt->utc_min_forcast,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,12,intMat2MatlabDoubles(&rpt->spare,1,1));

                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
            default:
            {//Error, reserved or unknown report type. Just return the
             //components common to all report types.
                const size_t numReportFields=5;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"report_type",
                "utc_day", "utc_hr", "utc_min", "site_id"};                      
                Ais8_1_26_SensorReport *rpt = reinterpret_cast<Ais8_1_26_SensorReport*>(msg.reports[curReport]);
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the components common to all sensor reports.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&rpt->report_type,1,1));
                if(rpt->utc_day!=0) {
                    mxSetFieldByNumber(reportStruct,0,1,intMat2MatlabDoubles(&rpt->utc_day,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_hr<=23) {
                    mxSetFieldByNumber(reportStruct,0,2,intMat2MatlabDoubles(&rpt->utc_hr,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(rpt->utc_min<=59) {
                    mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&rpt->utc_min,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,3,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&rpt->site_id,1,1));
                
                mxSetCell(reportsCellArray,curReport,reportStruct);
            }
                break;
        }
    }
    
    return AIS_OK;
}

AIS_STATUS AIS_8_1_27_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Route Information (Broadcast)"
    const size_t numberOfFields=15;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "link_id",
    "sender_type", "route_type", "utc_month", "utc_day", "utc_hour",
    "utc_min", "duration", "waypoints"};
    const size_t numberOfWaypointFields=2;
    mwSize waypointDims[2];
    const char *waypointFieldNames[numberOfWaypointFields] = {"x", "y"};
    mxArray *theStruct, *waypointArray;
    size_t curWaypoint;

    Ais8_1_27 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
       
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "A source specific running number (1-1023), unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID and the first six digits of the source MMSI uniquely identify the sent message.",//6
            "Sender Classification\n0 = ship = default\n1 = authority\n2 - 7 (reserved for future use)",//7
            "Route type:\n1 = mandatory route\n2 = recommended route\n3 = alternative route\n4 = recommended route through ice\n5 = ship route plan\n6 - 30 (reserved for future use)\n31 = cancellation (cancel route as identified by Message Linkage ID)",//8
            "UTC Month (1-12)",//9
            "UTC Day (1-31)",//10
            "UTC Hour (0-23)",//11
            "UTC minute (0-59)",//12
            "Minutes until end of validity of the route. Measured from start time of Route Information. 0 = cancel route",//13
            "An array containing WGS-84 longitude East and latitude North coordinates in degrees of the waypoints"//14
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.sender_type,1,1));
    if(msg.route_type!=0) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.route_type,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_month!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.utc_month,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_day!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_day,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_hour<=23) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_hour,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_min<=59) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_min,1,1));
    } else{
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.duration,1,1));

    waypointDims[0]=msg.waypoints.size();
    waypointDims[1]=1;
    waypointArray=mxCreateStructArray(2, waypointDims, numberOfWaypointFields, waypointFieldNames);
    mxSetFieldByNumber(theStruct,0,14,waypointArray);

    for(curWaypoint= 0; curWaypoint < msg.waypoints.size(); curWaypoint++) {
        if(fabs(msg.waypoints[curWaypoint].x)<=180) {
            mxSetFieldByNumber(waypointArray,curWaypoint,0,floatMat2MatlabDoubles(&msg.waypoints[curWaypoint].x,1,1));
        } else {
            mxSetFieldByNumber(waypointArray,curWaypoint,0,mxCreateDoubleMatrix(0,0,mxREAL));
        }
        if(msg.waypoints[curWaypoint].y<=90) {
            mxSetFieldByNumber(waypointArray,curWaypoint,1,floatMat2MatlabDoubles(&msg.waypoints[curWaypoint].y,1,1));
        } else {
            mxSetFieldByNumber(waypointArray,curWaypoint,1,mxCreateDoubleMatrix(0,0,mxREAL));
        }
    }
    
    return AIS_OK;
}

AIS_STATUS AIS_8_1_29_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Text description (broadcast)"
    const size_t numberOfFields=9;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "link_id", "text",
    "spare2"};
    mxArray *theStruct;
    
    Ais8_1_29 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Message Linkage ID. Used to link the Text Description message with a main message. The Connection ID and source MMSI MID uniquely identifies the main message. (1-1023)",
            "Text String"
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }

    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    {
        const char *charString=msg.text.c_str();
        mxSetFieldByNumber(theStruct,0,7,mxCreateCharMatrixFromStrings(1,&charString));
    }
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}


AIS_STATUS AIS_8_1_31_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Meteorological and Hydrographic data"
    const size_t numberOfFields=44;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "x", "y",
    "position_accuracy", "utc_day", "utc_hour", "utc_min", "wind_ave",
    "wind_gust", "wind_dir", "wind_gust_dir", "air_temp", "rel_humid",
    "dew_point", "air_pres", "air_pres_trend", "horz_vis", "water_level",
    "water_level_trend", "surf_cur_speed", "surf_cur_dir", "cur_speed_2",
    "cur_dir_2", "cur_depth_2", "cur_speed_3", "cur_dir_3", "cur_depth_3",
    "wave_height", "wave_period", "wave_dir", "swell_height", "swell_period",
    "swell_dir", "sea_state", "water_temp", "precip_type", "salinity",
    "ice", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_31 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Longitude East in degrees (WGS-84)",//6
            "Latitude North in degrees (WGS-84)",//7
            "Position accuracy. 1=high (<=10m), 0=low (>10m), 0=default",//8
            "UTC day transmitted (1-31)",//9
            "UTC hour transmitted (0-23)",//10
            "UTC minute transmitted (0-59)",//11
            "Average wind speed for the last 10 minutes in knots (0-126)",//12
            "Maximum wind gust speed during the last 10 minutes in knots (0-126)",//13
            "Wind direction in degrees East of North (0-359)",//14
            "Wind gust direction in degrees East of North (0-359)",//15
            "Dry buld air temperature in degrees Celsius (-60 to 60)",//16
            "Relative humidity in percent (0-100)",//17
            "Dew point in degrees Celsius (-20 to 50)",//18
            "Air pressure in hectoPascals (799-1201)",//19
            "Air pressure tendency 0 = steady, 1 = decreasing, 2 = increasing",//20
            "Horizontal visibility in nautical miles (0-12.6)",//21 NOTE: The first bit being set makes this an invalid value, and indicates that the equipment was saturated. We are not using that; an empty matrix is just returned.
            "Water level, including tide, deviation from local datum in meters (-10 to 30)",//22
            "Water level trend 0 = steady, 1 = decreasing, 2 = increasing",//23
            "Surface current speed including tide in knots (0-25.1)",//24
            "Surface current direction in degrees East of North (0-359)",//25
            "Current 2 speed in knots (0-25.1)",//26
            "Current 2 speed direction in degrees East of North (0-359)",//27
            "Depth of current 2 in meters (0-30)",//28
            "Current 3 speed in knots (0-25.1)",//29
            "Current 3 speed direction in degrees East of North (0-359)",//30
            "Depth of current 3 in meters (0-30)",//31
            "Significant wave height in meters (0-25)",//32
            "Wave period in seconds (0-60)",//33
            "Wave direction in degrees East of North (0-359)",//34
            "Swell height in meters (0-25)",//35
            "Swell period in seconds (0-60)",//36
            "Swell direction in degrees East of North (0-359)",//37
            "Sea state according to the Beauford scale (0-12)",//38
            "Water temperature in degrees Celsius (-10 to 50)",//39
            "Precipitation type according to the WMO\n0 = reserved\n1 = rain\n2 = thunderstorm\n3 = freezing rain\n4 = mixed/ice\n5 = snow\n6 = reserved",//40
            "Salinity in parts per thousand (0-50.1)",//41
            "A boolean value indicating the presence of ice",//42
            "Not used. Should be zero."//43
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
   if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {//No longitude
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {//No latitude
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.position_accuracy,1,1));
    
    if(msg.utc_day!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.utc_day,1,1));
    } else {//Day of departure not available
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_hour<24) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_hour,1,1));
    } else {//Hour of departure not available
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.utc_min<60) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_min,1,1));
    } else {//Minute of departure not available
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_ave<=126) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.wind_ave,1,1));
    } else {//No wind average
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_gust<=126) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.wind_gust,1,1));
    } else {//No wind gust speed
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_dir<=359) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.wind_dir,1,1));
    } else {//No wind direction
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wind_gust_dir<=359) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.wind_gust_dir,1,1));
    } else {//No wind gust direction
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.air_temp<=60&&msg.air_temp>=-60) {
        mxSetFieldByNumber(theStruct,0,16,floatMat2MatlabDoubles(&msg.air_temp,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,16,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.rel_humid<=100) {
        mxSetFieldByNumber(theStruct,0,17,intMat2MatlabDoubles(&msg.rel_humid,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,17,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.dew_point<=50&&msg.dew_point>=-20) {
        mxSetFieldByNumber(theStruct,0,18,floatMat2MatlabDoubles(&msg.dew_point,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,18,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.air_pres<=1201) {
        mxSetFieldByNumber(theStruct,0,19,intMat2MatlabDoubles(&msg.air_pres,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,19,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.air_pres_trend<=2){
        mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.air_pres_trend,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.horz_vis<=12.6) {
        mxSetFieldByNumber(theStruct,0,21,floatMat2MatlabDoubles(&msg.horz_vis,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,21,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.water_level<=30&&msg.water_level>=-10) {
        mxSetFieldByNumber(theStruct,0,22,floatMat2MatlabDoubles(&msg.water_level,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,22,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.water_level_trend<=2) {
        mxSetFieldByNumber(theStruct,0,23,intMat2MatlabDoubles(&msg.water_level_trend,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.surf_cur_speed<=25.1) {
        mxSetFieldByNumber(theStruct,0,24,floatMat2MatlabDoubles(&msg.surf_cur_speed,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.surf_cur_dir<=359) {
        mxSetFieldByNumber(theStruct,0,25,intMat2MatlabDoubles(&msg.surf_cur_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,25,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_speed_2<=25.1) {
        mxSetFieldByNumber(theStruct,0,26,floatMat2MatlabDoubles(&msg.cur_speed_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,26,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_dir_2<=359) {
        mxSetFieldByNumber(theStruct,0,27,intMat2MatlabDoubles(&msg.cur_dir_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,27,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_depth_2<=30) {
        mxSetFieldByNumber(theStruct,0,28,intMat2MatlabDoubles(&msg.cur_depth_2,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,28,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_speed_3<=25) {
        mxSetFieldByNumber(theStruct,0,29,floatMat2MatlabDoubles(&msg.cur_speed_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,29,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_dir_3<=359) {
        mxSetFieldByNumber(theStruct,0,30,intMat2MatlabDoubles(&msg.cur_dir_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,30,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.cur_depth_3<=30) {
        mxSetFieldByNumber(theStruct,0,31,intMat2MatlabDoubles(&msg.cur_depth_3,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,31,mxCreateDoubleMatrix(0,0,mxREAL));
    }
    if(msg.wave_height<=25.1) {
        mxSetFieldByNumber(theStruct,0,32,floatMat2MatlabDoubles(&msg.wave_height,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,32,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.wave_period<=60) {
        mxSetFieldByNumber(theStruct,0,33,intMat2MatlabDoubles(&msg.wave_period,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,33,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.wave_dir<=359) {
        mxSetFieldByNumber(theStruct,0,34,intMat2MatlabDoubles(&msg.wave_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,34,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.swell_height<=25.1) {
        mxSetFieldByNumber(theStruct,0,35,floatMat2MatlabDoubles(&msg.swell_height,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,35,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.swell_period<=60) {
        mxSetFieldByNumber(theStruct,0,36,intMat2MatlabDoubles(&msg.swell_period,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,36,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.swell_dir<=359) {
        mxSetFieldByNumber(theStruct,0,37,intMat2MatlabDoubles(&msg.swell_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,37,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.sea_state<=12) {
        mxSetFieldByNumber(theStruct,0,38,intMat2MatlabDoubles(&msg.sea_state,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,38,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.water_temp<=50&&msg.water_temp>=-10) {
        mxSetFieldByNumber(theStruct,0,39,floatMat2MatlabDoubles(&msg.water_temp,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,39,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.precip_type<7) {
        mxSetFieldByNumber(theStruct,0,40,intMat2MatlabDoubles(&msg.precip_type,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,40,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.salinity<=50) {
        mxSetFieldByNumber(theStruct,0,41,floatMat2MatlabDoubles(&msg.salinity,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,41,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.ice==0||msg.ice==1) {
        mxSetFieldByNumber(theStruct,0,42,intMat2MatlabDoubles(&msg.ice,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,42,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }

    mxSetFieldByNumber(theStruct,0,43,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

/*
AIS_STATUS AIS_8_1_40_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message format is described in Annex 5 of ITU-R M.1371-1. It is not
//present in ITU-R M.1371-5
    const size_t numberOfFields=8;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id", "repeat_indicator",
    "mmsi", "spare", "dac", "fi", "persons", "spare2"};
    mxArray *theStruct;
    
    Ais8_1_40 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Current number of persons on board, including crew members (1-8191)",//6
            "Not used. Should be set to zero"//7
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));

    //Fields specific to the Ais8_1_40 type.
    if(msg.persons!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.persons,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}
*/
AIS_STATUS AIS_8_200_10_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message is defined in ECE-TRANS-SC3-176e.pdf
//Message "Inland ship static and voyage related data"
    const size_t numberOfFields=17;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "eu_id", "length",
    "beam", "ship_type", "haz_cargo", "draught", "loaded", "speed_qual",
    "course_qual", "heading_qual", "spare2"};
    mxArray *theStruct;
    
    Ais8_200_10 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "European Identifier string",
            "Length of ship in meters (0=default)",
            "Beam of ship in meters (0=default)",
            "Numeric ERI Classification for ship or combination type",
            "Hazardous Cargo, number of blue cones/ lights0-3, 4=B-flag",
            "Draught in meters"
            "1=loaded, 2=unloaded, 3 should not be used",
            "Quality of speed information1=high, 0=low/GNSS",
            "Quality of course information1=high, 0=low/GNSS",
            "Quality of heading information1=high, 0=low",
            "Not used, should be set to zero. Reserved for future use."
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    {
        const char *charString=msg.eu_id.c_str();
        mxSetFieldByNumber(theStruct,0,6,mxCreateCharMatrixFromStrings(1,&charString));
    }
    mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.length,1,1));
    mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(&msg.beam,1,1));
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.ship_type,1,1));
    if(msg.haz_cargo<=4) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.haz_cargo,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.draught!=0) {
        mxSetFieldByNumber(theStruct,0,11,floatMat2MatlabDoubles(&msg.draught,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.loaded!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.loaded,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.speed_qual,1,1));
    mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.course_qual,1,1));
    mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.heading_qual,1,1));
    mxSetFieldByNumber(theStruct,0,16,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}

/*
AIS_STATUS AIS_8_200_23_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions,const char *nmea_payload, const size_t pad) {
//This message is defined in ECE-TRANS-SC3-176e.pdf
//Message "EMMA warning"
    const size_t numberOfFields=26;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "utc_year_start",
    "utc_month_start", "utc_day_start", "utc_hour_start", "utc_min_start"
    "utc_year_end", "utc_month_end", "utc_day_end", "utc_hour_end",
    "utc_min_end", "x1", "y1", "x2", "y2", "type", "min", "max",
    "classification", "wind_dir", "spare2"};
    mxArray *theStruct;
    
    Ais8_200_23 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "UTC year of the starting validity period given as an offset from the year 2000 (1-255)",//6
            "UTC month of the starting validity period (1-12)",//7
            "UTC day of the starting validity period (1-31)",//8
            "UTC hour of the starting validity period (0-23)",//9
            "UTC minute of the starting validity period (0-59)",//10
            "UTC year of the ending validity period given as an offset from the year 2000 (1-255)",//11
            "UTC month of the ending validity period (1-12)",//12
            "UTC day of the ending validity period (1-31)",//13
            "UTC hour of the ending validity period (0-23)",//14
            "UTC minute of the ending validity period (0-59)",//15
            "Longitude East in degrees for the beginning of the fairway section (WGS-84)",//16
            "Latitude North in degrees for the beginning of the fairway section (WGS-84)",//17
            "Longitude East in degrees for the end of the fairway section (WGS-84)",//18
            "Latitude North in degrees for the end of the fairway section (WGS-84)",//19
            "Type of weather warning. Possible values are:\n1 Wind\n2 Rain\n3 Snow and Ice\n4 Thunderstorm\n5 Fog\n6 Low Temperature\n7 High Temperature\n8 Flood\n9 Fire in the Forests",//20
            "Min Value",//21
            "Max Value",//22
            "Classification of warning\n1  slight\n2  medium\n3 strong/heavy",//23
            "Wind direction such that\n1 North\n2 North East\n3 East\n4 South East\n5 South\n6 South West\n7 West\n8 North West",//24
            "Not used, should be set to zero. Reserved for future use."//25
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    if(msg.utc_year_start!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.utc_year_start,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_month_start!=0) {
        mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.utc_month_start,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_day_start!=0) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.utc_day_start,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_hour_start<=23) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.utc_hour_start,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_min_start<=59) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.utc_min_start,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_year_end!=0) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.utc_year_end,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_month_end!=0) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.utc_month_end,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_day_end!=0) {
        mxSetFieldByNumber(theStruct,0,13,intMat2MatlabDoubles(&msg.utc_day_end,1,1)); 
    } else {
        mxSetFieldByNumber(theStruct,0,13,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_hour_end<=23) {
        mxSetFieldByNumber(theStruct,0,14,intMat2MatlabDoubles(&msg.utc_hour_end,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,14,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.utc_min_end<=59) {
        mxSetFieldByNumber(theStruct,0,15,intMat2MatlabDoubles(&msg.utc_min_end,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,15,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,16,floatMat2MatlabDoubles(&msg.x1,1,1));
    mxSetFieldByNumber(theStruct,0,17,floatMat2MatlabDoubles(&msg.y1,1,1));
    mxSetFieldByNumber(theStruct,0,18,floatMat2MatlabDoubles(&msg.x2,1,1));
    mxSetFieldByNumber(theStruct,0,19,floatMat2MatlabDoubles(&msg.y2,1,1));
    if(msg.type!=0){
        mxSetFieldByNumber(theStruct,0,20,intMat2MatlabDoubles(&msg.type,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,20,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,21,intMat2MatlabDoubles(&msg.min,1,1));
    mxSetFieldByNumber(theStruct,0,22,intMat2MatlabDoubles(&msg.max,1,1));
    if(msg.classification!=0) {
        mxSetFieldByNumber(theStruct,0,23,intMat2MatlabDoubles(&msg.classification,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,23,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.wind_dir!=0) {
        mxSetFieldByNumber(theStruct,0,24,intMat2MatlabDoubles(&msg.wind_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,24,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,25,intMat2MatlabDoubles(&msg.spare2,1,1));

    return AIS_OK;
}
*/

AIS_STATUS AIS_8_200_24_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message is defined in ECE-TRANS-SC3-176e.pdf
//Message "Water levels"
    const size_t numberOfFields=9;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "country",
    "gauge_ids", "levels"};
    mxArray *theStruct;
    
    Ais8_200_24 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "United Nations country code string according to ERI specification",//6
            "An array of four gauge IDs, (0-2047) 0 means unknown.",//7
            "An array of water levels in meters (0-204.7), 0 means unknown",//8
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    {
        const char *charString=msg.country.c_str();
        mxSetFieldByNumber(theStruct,0,6,mxCreateCharMatrixFromStrings(1,&charString));
    }
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(msg.gauge_ids,4,1));
    mxSetFieldByNumber(theStruct,0,8,floatMat2MatlabDoubles(msg.levels,4,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_200_40_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message is defined in ECE-TRANS-SC3-176e.pdf
//Message "Signal Status"
    const size_t numberOfFields=13;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "x", "y", "form",
    "dir", "stream_dir", "status_raw", "spare2"};
    mxArray *theStruct;
    
    Ais8_200_40 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Signal position East longitue in degrees (WGS-84)",//6
            "Signal position North latitude in degrees (WGS-84)",//7
            "Signal form according to Annex C of ECE-TRANS-SC3-2006-10e.pdf",//8
            "Orientation of signal in degrees East of North (0-359)",//9
            "Direction of impact:\n1 Upstream\n2 Downstream\n3 To the left bank\n4 To the right bank",//10
            "The light status as per ECE-TRANS-SC3-2006-10e.pdf",//11
            "Not used, should be set to zero. Reserved for future use."//12
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    if(fabs(msg.x)<=180) {
        mxSetFieldByNumber(theStruct,0,6,floatMat2MatlabDoubles(&msg.x,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(fabs(msg.y)<=90) {
        mxSetFieldByNumber(theStruct,0,7,floatMat2MatlabDoubles(&msg.y,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.form!=0&&msg.form!=15) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.form,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.dir<=359) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.stream_dir!=0) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.stream_dir,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.status_raw,1,1));
    mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.spare2,1,1));
    
    return AIS_OK;
}

AIS_STATUS AIS_8_200_55_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message is defined in ECE-TRANS-SC3-176e.pdf
//Message "number of persons on board"
    const size_t numberOfFields=10;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "crew", "passengers",
    "yet_more_personnel", "spare2"};
    mxArray *theStruct;
    
    Ais8_200_55 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
     
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "Numer of crew members on board (0-254)",//6
            "Number of passengers on board (0-8190)",//7
            "Number of shipboard personnel on board (0-254)",//8
            "Not used, should be set to zero. Reserved for future use."//9
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    if(msg.crew!=255) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.crew,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.passengers!=8191) {
        mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.passengers,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,7,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.yet_more_personnel!=255) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.yet_more_personnel,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(msg.spare2,3,1));

    return AIS_OK;
}

AIS_STATUS AIS_8_1_22_ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This message format is described in IMO circular 289.
//Message "Area notice (broadcast)"
    const size_t numberOfFields=14;
    const mwSize dims[2] = {1, 1};
    mwSize cellArrayDims[2];
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi", "link_id",
    "notice_type", "month", "day", "hour", "minute", "duration_minutes",
    "sub_areas"};
    mxArray *theStruct, *subareaCellArray;
    size_t curSubarea;

    Ais8_001_22 msg(nmea_payload, pad);
    if (msg.had_error()) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
            "A source specific running number (1-1023), unique across all binary messages equipped with Message Linkage ID. Used to link additional information to the message by a Text Description message. The Message Linkage ID and the first six digits of the source MMSI uniquely identify the sent message.",//6
            "Notice Description, 0-127 as per table 11.11 in IMO circular 289",
            "UTC month (1-12)",
            "UTC day (1-31)",
            "UTC hour (0-23)",
            "UTC minute (0-59)",
            "Duration in minutes until the end of the area notice measured from the start date and time of the notice. Max is 262142",
            "A structure of elements for the subareas as defined in IMO circular 289. The subareas define shapes"
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));
    
    //Fill in the components unique to this message.
    if(msg.link_id!=0) {
        mxSetFieldByNumber(theStruct,0,6,intMat2MatlabDoubles(&msg.link_id,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,6,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    mxSetFieldByNumber(theStruct,0,7,intMat2MatlabDoubles(&msg.notice_type,1,1));
    if(msg.month!=0) {
        mxSetFieldByNumber(theStruct,0,8,intMat2MatlabDoubles(&msg.month,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,8,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.day!=0) {
        mxSetFieldByNumber(theStruct,0,9,intMat2MatlabDoubles(&msg.day,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,9,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.hour<=23) {
        mxSetFieldByNumber(theStruct,0,10,intMat2MatlabDoubles(&msg.hour,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,10,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.minute<=59) {
        mxSetFieldByNumber(theStruct,0,11,intMat2MatlabDoubles(&msg.minute,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,11,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }
    if(msg.duration_minutes!=262143) {
        mxSetFieldByNumber(theStruct,0,12,intMat2MatlabDoubles(&msg.duration_minutes,1,1));
    } else {
        mxSetFieldByNumber(theStruct,0,12,mxCreateDoubleMatrix(0,0,mxREAL)); 
    }

    //The subareas are put into a cell array.
    cellArrayDims[0]=msg.sub_areas.size();
    cellArrayDims[1]=1;
    //Make space to hold all of the reports in a cell array.
    subareaCellArray=mxCreateCellArray(2, cellArrayDims);
    mxSetFieldByNumber(theStruct,0,13,subareaCellArray);

    //Fill in all of the reports.
    for(curSubarea=0;curSubarea<msg.sub_areas.size();curSubarea++) {
        int area_shape=msg.sub_areas[curSubarea]->getType();
        switch(area_shape) {
            case AIS8_001_22_SHAPE_CIRCLE:
            {
                const size_t numReportFields=6;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape",
                "x", "y", "precision", "radius_m", "spare"};                      
                mxArray *reportStruct;
                Ais8_001_22_Circle *c =reinterpret_cast<Ais8_001_22_Circle*>(msg.sub_areas[curSubarea]);
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));

                //Fill in the components for this subarea
                mxSetFieldByNumber(reportStruct,0,1,floatMat2MatlabDoubles(&c->x,1,1));
                mxSetFieldByNumber(reportStruct,0,2,floatMat2MatlabDoubles(&c->y,1,1));
                mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&c->precision,1,1));
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&c->radius_m,1,1));
                mxSetFieldByNumber(reportStruct,0,5,mat2MatlabDoubles(&c->spare,1,1));
                
                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
            case AIS8_001_22_SHAPE_RECT:
            {
                const size_t numReportFields=8;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape",
                "x", "y", "precision", "e_dim_m", "n_dim_m", "orient_deg",
                "spare"};                      
                mxArray *reportStruct;
                Ais8_001_22_Rect *c =reinterpret_cast<Ais8_001_22_Rect*>(msg.sub_areas[curSubarea]);
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));

                //Fill in the components for this subarea
                if(fabs(c->x)<=180) {
                    mxSetFieldByNumber(reportStruct,0,1,floatMat2MatlabDoubles(&c->x,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(fabs(c->y)<=90) {
                    mxSetFieldByNumber(reportStruct,0,2,floatMat2MatlabDoubles(&c->y,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&c->precision,1,1));
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&c->e_dim_m,1,1));
                mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&c->n_dim_m,1,1));
                mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&c->orient_deg,1,1));
                mxSetFieldByNumber(reportStruct,0,7,mat2MatlabDoubles(&c->spare,1,1));
                
                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
            case AIS8_001_22_SHAPE_SECTOR:
            {
                const size_t numReportFields=7;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape",
                "x", "y", "precision", "radius_m", "left_bound_deg",
                "right_bound_deg"};                      
                mxArray *reportStruct;
                Ais8_001_22_Sector *c =reinterpret_cast<Ais8_001_22_Sector*>(msg.sub_areas[curSubarea]);
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));

                //Fill in the components for this subarea
                if(fabs(c->x)<=180) {
                    mxSetFieldByNumber(reportStruct,0,1,floatMat2MatlabDoubles(&c->x,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                if(fabs(c->y)<=90) {
                    mxSetFieldByNumber(reportStruct,0,2,floatMat2MatlabDoubles(&c->y,1,1));
                } else {
                    mxSetFieldByNumber(reportStruct,0,2,mxCreateDoubleMatrix(0,0,mxREAL));
                }
                mxSetFieldByNumber(reportStruct,0,3,intMat2MatlabDoubles(&c->precision,1,1));
                mxSetFieldByNumber(reportStruct,0,4,intMat2MatlabDoubles(&c->radius_m,1,1));
                mxSetFieldByNumber(reportStruct,0,5,intMat2MatlabDoubles(&c->left_bound_deg,1,1));
                mxSetFieldByNumber(reportStruct,0,6,intMat2MatlabDoubles(&c->right_bound_deg,1,1));

                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
            case AIS8_001_22_SHAPE_POLYLINE:
            {
                const size_t numReportFields=4;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape",
                "angles", "dists_m", "spare"};                      
                mxArray *reportStruct, *angleArray, *distArray;
                Ais8_001_22_Polyline *c =reinterpret_cast<Ais8_001_22_Polyline*>(msg.sub_areas[curSubarea]);
                size_t curItem;
                double *data;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));

                //Fill in the components for this subarea
                angleArray=mxCreateDoubleMatrix(c->angles.size(),1,mxREAL);
                data=(double*)mxGetData(angleArray);
                for(curItem=0;curItem<c->angles.size();curItem++) {
                    data[curItem]=c->angles[curItem];
                }
                mxSetFieldByNumber(reportStruct,0,1,angleArray);
                
                distArray=mxCreateDoubleMatrix(c->dists_m.size(),1,mxREAL);
                data=(double*)mxGetData(distArray);
                for(curItem=0;curItem<c->dists_m.size();curItem++) {
                    data[curItem]=c->dists_m[curItem];
                }
                mxSetFieldByNumber(reportStruct,0,2,distArray);
                
                mxSetFieldByNumber(reportStruct,0,3,mat2MatlabDoubles(&c->spare,1,1));

                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
            case AIS8_001_22_SHAPE_POLYGON:
                        {
                const size_t numReportFields=4;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape",
                "angles", "dists_m", "spare"};                      
                mxArray *reportStruct,*angleArray, *distArray;
                Ais8_001_22_Polygon *c =reinterpret_cast<Ais8_001_22_Polygon*>(msg.sub_areas[curSubarea]);
                size_t curItem;
                double *data;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));

                //Fill in the components for this subarea
                angleArray=mxCreateDoubleMatrix(c->angles.size(),1,mxREAL);
                data=(double*)mxGetData(angleArray);
                for(curItem=0;curItem<c->angles.size();curItem++) {
                    data[curItem]=c->angles[curItem];
                }
                mxSetFieldByNumber(reportStruct,0,1,angleArray);
                
                distArray=mxCreateDoubleMatrix(c->dists_m.size(),1,mxREAL);
                data=(double*)mxGetData(distArray);
                for(curItem=0;curItem<c->dists_m.size();curItem++) {
                    data[curItem]=c->dists_m[curItem];
                }
                mxSetFieldByNumber(reportStruct,0,2,distArray);
                
                mxSetFieldByNumber(reportStruct,0,3,mat2MatlabDoubles(&c->spare,1,1));

                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
            case AIS8_001_22_SHAPE_TEXT:
            {
                //save the area shape number.
                const size_t numReportFields=2;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape",
                "text"};                      
                mxArray *reportStruct;
                Ais8_001_22_Text *c =reinterpret_cast<Ais8_001_22_Text*>(msg.sub_areas[curSubarea]);

                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));
                
                //Fill in the components for this subarea
                {
                    const char *charString=c->text.c_str();
                    mxSetFieldByNumber(reportStruct,0,1,mxCreateCharMatrixFromStrings(1,&charString));
                }

                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
            default://An error or an unknown shape. In this instance, just
            {
                //save the area shape number.
                const size_t numReportFields=1;
                const mwSize reportDims[2] = {1, 1};
                const char *reportFieldNames[numReportFields]={"area_shape"};                      
                mxArray *reportStruct;
                
                //Create the Matlab structure array.
                reportStruct=mxCreateStructArray(2, reportDims, numReportFields, reportFieldNames);
                //Fill in the component common to all subareas.
                mxSetFieldByNumber(reportStruct,0,0,intMat2MatlabDoubles(&area_shape,1,1));
                
                mxSetCell(subareaCellArray,curSubarea,reportStruct);
            }
                break;
        }
    }

    return AIS_OK;   
}

AIS_STATUS AIS8ToMatlab(mxArray **matlabStruct, mxArray **fieldDescriptions, const char *nmea_payload, const size_t pad) {
//This is only called if no subtype can be identified
    const size_t numberOfFields=6;
    const mwSize dims[2] = {1, 1};
    //These are the names of all of the fields that will be added to the
    //Matlab structre array.
    const char *fieldNames[numberOfFields] = {"message_id",
    "repeat_indicator", "mmsi", "spare", "dac", "fi"};
    mwSize cellArrayDims[2];
    mxArray *theStruct, *reportsCellArray;
    size_t curReport;
    
    Ais8 msg(nmea_payload, pad);
    if (msg.had_error()&&msg.get_error()!=AIS_UNINITIALIZED) {
        return msg.get_error();
    }
    
    //If text descriptions of the fields are desired, then put them into a
    //structure. These are just the descriptions of fields common to all
    //AIS-8 messages.
    if(fieldDescriptions!=NULL) {
        const char *descriptionStrings[numberOfFields] = {
            "Identifier for Message 8; always 8.",//0
            "Repeat indicator. Used by the repeater to indicate how many times a message has been repeated. 0 = default; 3 = do not repeat any more.",//1
            "Maritime mobile service identity (MMSI) number of source station",//2
            "Not used. Should be zero. Reserved for future use",//3
            "Designated area code (DAC)",//4
            "Function identifier (FI)",//5
        };
        int i;

        *fieldDescriptions=mxCreateStructArray(2, dims, numberOfFields, fieldNames);

        for(i=0;i<numberOfFields;i++) {
            mxSetFieldByNumber(*fieldDescriptions,0,i,mxCreateCharMatrixFromStrings(1,&descriptionStrings[i]));
        }
    }
    
    //Create the Matlab structure array.
    theStruct=mxCreateStructArray(2, dims, numberOfFields, fieldNames);
    *matlabStruct=theStruct;
    
    //Fill all of the elements of the structure with the set that is common
    //to all AIS8 messages.
    mxSetFieldByNumber(theStruct,0,0,intMat2MatlabDoubles(&msg.message_id,1,1));
    mxSetFieldByNumber(theStruct,0,1,intMat2MatlabDoubles(&msg.repeat_indicator,1,1));
    mxSetFieldByNumber(theStruct,0,2,intMat2MatlabDoubles(&msg.mmsi,1,1));
    mxSetFieldByNumber(theStruct,0,3,intMat2MatlabDoubles(&msg.spare,1,1));
    mxSetFieldByNumber(theStruct,0,4,intMat2MatlabDoubles(&msg.dac,1,1));
    mxSetFieldByNumber(theStruct,0,5,intMat2MatlabDoubles(&msg.fi,1,1));

    return AIS_OK;
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
