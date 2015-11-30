/**
 * @brief
 *    Box Type: 'ftyp' - File type Box
 *    Container: File
 *    Mandatory: Yes
 *    Quantity: Exactly one
 *
 *    A media-file structured to this part of this specification may be
 *    compatible with more than one detailed specification, and it is
 *    therefore not always possible to speak of a single ‘type’ or ‘brand’
 *    for the file. This means that the utility of the file name extension
 *    and mime type are somewhat reduced.
 *
 *    This box must be placed as early as possible in the file (e.g. after
 *    any obligatory signature, but before any significant variable-size
 *    boxes such as a Movie Box, Media Data Box, or Free Space). It identi-
 *    fies which specification is the ‘best use’ of the file, and a minor
 *    version of that specification; and also a set of other specifications
 *    to which the file complies. Readers implementing this format should
 *    attempt to read files that are marked as compatible with any of the
 *    specifications that the reader implements. Any incompatible change in
 *    a specification should therefore register a new ‘brand’ identifier to
 *    identify files conformant to the new specification.
 *
 *    The minor version is informative only. It does not appear for compat-
 *    ible-brands, and must not be used to determine the conformance of a
 *    file to a standard. It may allow more precise identification of the
 *    major specification, for inspection, debugging, or improved decoding.
 *
 *    The type ‘isom’ (ISO Base Media file) is defined in this section of
 *    this specification, as identifying files that conform to the first
 *    version of ISO Base Media File Format.
 *
 *    More specific identifiers can be used to identify precise versions of
 *    specifications providing more detail. This brand should not be used as
 *    the major brand; this base file format should be derived into another
 *    specification to be used. There is therefore no defined normal file
 *    extension, or mime type assigned to this brand, nor definition of the
 *    minor version when ‘isom’ is the major brand.
 *
 *    Files would normally be externally identified (e.g. with a file exten-
 *    sion or mime type) that identifies the ‘best use’ (major brand), or
 *    the brand that the author believes will provide the greatest compati-
 *    bility.
 *
 *    The brand ‘iso2’ shall be used to indicate compatibility with this
 *    amended version of the ISO Base Media File Format; it may be used in
 *    addition to or instead of the ‘isom’ brand and the same usage rules
 *    apply. If used without the brand 'isom' identifying the first version
 *    of this specification, it indicates that support for some or all of
 *    the technology introduced by this amendment is required, such as the
 *    functionality in sub-clauses [8.40] through [8.45], or the SRTP
 *    support in sub-clause [10], is required.
 *
 *    The brand ‘avc1’ shall be used to indicate that the file is conforma-
 *    nt with the ‘AVC Extensions’ in sub-clause [8.40]. If used without
 *    other brands, this implies that support for those extensions is requ-
 *    ired. The use of ‘avc1’ as a major-brand may be permitted by specifi-
 *    cations; in that case, that specification defines the file extension
 *    and required behavior.
 *
 *    If a Meta-box with an MPEG-7 handler type is used at the file level,
 *    then the brand ‘mp71’ should be a member of the compatible-brands
 *    list in the file-type box.
 * */
#ifndef FTYP_BOX_H_
#define FTYP_BOX_H_

#include "Mp4.h"

#define MAX_FTYP_BOXSIZE               100

typedef struct Tag_Ftyp_Box {
    Box_Header * boxheader;
    unsigned int major_brand;
    unsigned int minor_version;
    unsigned int * compatible_brands; // to end of the box
} Ftyp_Box;

int AllocStruct_Ftype_Box(Ftyp_Box ** ftypbox);
int FreeStruct_Ftype_Box(Ftyp_Box * ftypbox);
int Write_Ftype_Ftyp_Box(unsigned char * Buf);

#endif
