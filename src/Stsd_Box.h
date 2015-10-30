/**
 * @brief
 *    Box Types: 'stsd'
 *    Container: Sample Table Box ('stbl')
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    The sample description table gives detailed information about the
 *    coding type used, and any initialization information needed for that
 *    coding.
 *    
 *    The information stored in the sample description box after the entry-
 *    count is both track-type specific as documented here, and can also
 *    have variants within a track type (e.g. different codings may use
 *    different specific information after some common fields, even within
 *    a video track).
 *    
 *    For video tracks, a VisualSampleEntry is used; for audio tracks, an
 *    AudioSampleEntry. Hint tracks use an entry format specific to their
 *    protocol, with an appropriate name.
 *    
 *    For hint tracks, the sample description contains appropriate declarative
 *    data for the streaming protocol being used, and the format of the hint
 *    track. The definition of the sample description is specific to the
 *    protocol.
 *    
 *    Multiple descriptions may be used within a track.
 *
 *    The 'protocol' and 'codingname' fields are registered identifiers that
 *    uniquely identify the streaming protocol or compression format decoder
 *    to be used. A given protocol or codingname may have optional or required
 *    extensions to the sample description (e.g. codec initialization parame-
 *    ters). All such extensions shall be within boxes; these boxes occur after
 *    the required fields. Unrecognized boxes shall be ignored.
 *
 *    If the 'format' field of a SampleEntry is unrecognized, neither the samp-
 *    le description itself, nor the associated media samples, shall be decod-
 *    ed.
 *
 *    In audio tracks, the sampling rate of the audio should be used as the
 *    timescale of the media, and also documented in the samplerate field here.
 *
 *    In video tracks, the frame_count field must be 1 unless the specification
 *    for the media format explicitly documents this template field and permits
 *    larger values. That specification must document both how the individual
 *    frames of video are found (their size information) and thieir timing
 *    established. That timing might be as simple as dividing the sample dura-
 *    tion by the frame count to establish the frame duration.
 * */
#ifndef STSD_BOX_H_
#define STSD_BOX_H_

#include "Mp4.h"

//AVCDecoderConfigurationRecord
typedef struct Tag_Avcc_Box {
    Box_Header * boxheader;
    unsigned char configurationVersion; //
    unsigned char AVCProfileIndication; //
    unsigned char profile_compatibility; //
    unsigned char AVCLevelIndication; //
    unsigned char reserved_1; //‘111111’b;
    unsigned char lengthSizeMinusOne; //
    unsigned char reserved_2; //‘111’b;
    unsigned char numOfSequenceParameterSets; //
    unsigned int sequenceParameterSetLength; //
    unsigned char * sequenceParameterSetNALUnit; //
    unsigned char numOfPictureParameterSets; //
    unsigned int pictureParameterSetLength; //
    unsigned char * pictureParameterSetNALUnit; //
    unsigned char reserved_3;
    unsigned char chroma_format;
    unsigned char reserved_4;
    unsigned char bit_depth_luma_minus8;
    unsigned char reserved_5;
    unsigned char bit_depth_chroma_minus8;
    unsigned char numOfSequenceParameterSetExt;
    unsigned int sequenceParameterSetExtLength;
    unsigned char * sequenceParameterSetExtNALUnit;
} Avcc_Box;

//AudioSpecificConfig
typedef struct Tag_AudioSpecificConfig_Box {
    Box_Header * boxheader;
    
    unsigned char audioObjectType; //
    unsigned char samplingFrequencyIndex; //
    unsigned char channelConfiguration; //
    unsigned char framelengthFlag; //
    unsigned char dependsOnCoreCoder; //
    unsigned char extensionFlag; //
} AudioSpecificConfig_Box;

//ISO/IEC 14496-1 ES_Descriptor is defined as follow:
//8.6.5.1 Syntax
//class ES_Descriptor extends BaseDescriptor : bit(8) tag=ES_DescrTag {
//	bit(16) ES_ID;
//	bit(1) streamDependenceFlag;
//	bit(1) URL_Flag;
//	bit(1) OCRstreamFlag;
//	bit(5) streamPriority;
//	if (streamDependenceFlag)
//		bit(16) dependsOn_ES_ID;
//	if (URL_Flag) {
//		bit(8) URLlength;
//		bit(8) URLstring[URLlength];
//	}
//	if (OCRstreamFlag)
//		bit(16) OCR_ES_Id;
//	DecoderConfigDescriptor decConfigDescr;
//	SLConfigDescriptor slConfigDescr;
//	IPI_DescrPointer ipiPtr[0 .. 1];
//	IP_IdentificationDataSet ipIDS[0 .. 255];
//	IPMP_DescriptorPointer ipmpDescrPtr[0 .. 255];
//	LanguageDescriptor langDescr[0 .. 255];
//	QoS_Descriptor qosDescr[0 .. 1];
//	RegistrationDescriptor regDescr[0 .. 1];
//	ExtensionDescriptor extDescr[0 .. 255];
//}

typedef struct Tag_Esds_Box {
    FullBox_Header * boxheader;
    unsigned char tag; //8
    unsigned char Length_Field; //8   Length Field：25
    unsigned int ES_ID; //bit(16)
    unsigned char streamDependenceFlag; //
    unsigned char URL_Flag; //
    unsigned char OCRstreamFlag; //
    unsigned char streamPriority; //
    unsigned char DecoderConfigDescriptor_tag; //
    unsigned char Length_Field_1; //8 = 0x11
    unsigned char objectTypeIndication; //8 = 0x40  14496-1 Table8, 0x40是Audio ISO/IEC 14496-3
    unsigned char streamType; //
    unsigned char upStream; //1
    unsigned char reserved; //1
    unsigned int bufferSizeDB; //24
    unsigned int maxBitrate; //32
    unsigned int avgBitrate; //32
    unsigned char DecSpecificInfotag; //8 = 0x05
    unsigned char Length_Field_2; //8  = 0x02
    unsigned char audioObjectType_2_GASpecificConfig; //5
    unsigned char samplingFrequencyIndex; //4
    unsigned char channelConfiguration; //4
    unsigned char cpConfig; //2
    unsigned char directMapping; // 1
    unsigned char SLConfigDescrTag; //8  = 0x06
    unsigned char Length_Field_3; //8    = 0x01
    unsigned char predefined; //8  predefined 0x02 Reserved for use in MP4 files
} Esds_Box;

// for video tracks; VisualSampleEntry();
typedef struct Tag_Stsd_Vide {
    unsigned char reserved_format[6];
    unsigned int data_reference_index;
    unsigned int pre_defined;
    unsigned int reserved;
    unsigned int pre_defined_1[3];
    
        // the maximum visual width and height of the stream described
        // by this sample description, in pixels
    unsigned int width;
    unsigned int height;
    
    unsigned int horizresolution; // 72 dpi
    unsigned int vertresolution; // 72 dpi
    unsigned int reserved_1;

        /*
         * indicates how many frames of compressed video are stored in each
         * sample. The default is 1, for one frame per sample; it may be
         * more than 1 for multiple frames per sample.
         * */
    unsigned int frame_count;

        /*
         * for informative purposes. It is formatted in a fixed 32-byte
         * field, with the first byte set to the number of bytes to be
         * displayed, followed by that number of bytes of displayable
         * data, and then padding to complete 32 bytes total (including
         * the size byte). The filed may be set to 0.
         * */
    unsigned char *compressorname[32];
    
        // takes one of the following values
        // 0x0018 - images are in color with no alpha
    unsigned int depth;
    
    int pre_defined_2;
    Avcc_Box * avccbox;
} Stsd_Vide;

// for audio tracks; AudioSampleEntry();
typedef struct Tag_Stsd_Soun {
    unsigned char reserved_format[6];
        /*
         * contains the index of the data reference to use to retrieve
         * data associated with samples that use this sample description.
         * Data references are stored in Data Reference Boxes. The index
         * ranges from 1 to the number of data references.
         * */
    unsigned int data_reference_index;
    unsigned int reserved[2];
        // either 1 (mono) or 2 (stereo)
    unsigned int channelcount;
        // int bits, and takes the default value of 16
    unsigned int samplesize;
    unsigned int pre_defined;
    unsigned int reserved_1;
        // the sampling rate expressed as a 16.16 fixed-point number (hi.lo)
    unsigned int samplerate;
    AudioSpecificConfig_Box * audiospecificconfigbox;
    Esds_Box * esdsbox;
} Stsd_Soun;

typedef struct Tag_Stsd_Box {
    FullBox_Header * boxheader;

        // gives the number of entries in the following table
    unsigned int entry_count;
    Stsd_Vide * std_vide;
    Stsd_Soun * stsd_soun;
} Stsd_Box;

int AllocStruct_Stsd_Box(Stsd_Box ** stsdbox);
int FreeStruct_Stsd_Box(Stsd_Box * stsdbox);

int AllocStruct_Stsd_Vide(Stsd_Vide ** stsdvide,
                          unsigned int entry_count_num);
int FreeStruct_Stsd_Vide(Stsd_Vide * stsdvide,
                         unsigned int entry_count_num);

int AllocStruct_Stsd_Soun(Stsd_Soun ** stsdsoun,
                          unsigned int entry_count_num);
int FreeStruct_Stsd_Soun(Stsd_Soun * stsdsoun,
                         unsigned int entry_count_num);

int Write_Stsd_Stsd_Box_Video(unsigned char * Buf,
                              unsigned int width_stsd,
                              unsigned int height_stsd,
                              unsigned char * spsbuf,
                              unsigned int spslength,
                              unsigned char * ppsbuf,
                              unsigned int ppslength);

int Write_Stsd_Stsd_Box_Audio_AudioSpecificConfig(unsigned char * Buf,
                                                  unsigned int channelcount_stsd,
                                                  unsigned int samplerate_stsd,
                                                  unsigned char audioObjectType_stsd);
int Write_Stsd_Stsd_Box_Audio_Mp4a_Esds(unsigned char * Buf,
                                        unsigned int channelcount_stsd,
                                        unsigned int samplerate_stsd,
                                        unsigned char audioObjectType_stsd);

#endif // STSD_BOX_H_
