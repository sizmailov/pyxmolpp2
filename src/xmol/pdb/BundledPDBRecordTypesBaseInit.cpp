#include "xmol/pdb/PdbRecord.h"

using namespace xmol::pdb;

std::map<RecordName,PdbRecordType> detail::get_bundled_records() {

  auto as_field = [](const std::string& fieldName, std::vector<int> colons){
    auto shortend = fieldName.substr(0,std::min(size_t(FieldName::max_length),fieldName.size()));
    return std::make_pair(FieldName(shortend),std::move(colons));
  };
  auto as_record = [](const char* recordName, std::map<FieldName,std::vector<int>> fields){
    return std::pair<RecordName,PdbRecordType>(RecordName(recordName),PdbRecordType(std::move(fields)));
  };
  std::map<RecordName,PdbRecordType> result {
      as_record("ANISOU",{
          as_field("serial",{7,11}),
          as_field("name",{13,16}),
          as_field("altLoc",{17,17}),
          as_field("resName",{18,20}),
          as_field("chainID",{22,22}),
          as_field("resSeq",{23,26}),
          as_field("iCode",{27,27}),
          as_field("u[0][0]",{29,35}),
          as_field("u[1][1]",{36,42}),
          as_field("u[2][2]",{43,49}),
          as_field("u[0][1]",{50,56}),
          as_field("u[0][2]",{57,63}),
          as_field("u[1][2]",{64,70}),
          as_field("element",{77,78}),
          as_field("charge",{79,80}),
      }), // ANISOU
      as_record("ATOM",{
          as_field("serial",{7,11}),
          as_field("name",{13,16}),
          as_field("altLoc",{17,17}),
          as_field("resName",{18,20}),
          as_field("chainID",{22,22}),
          as_field("resSeq",{23,26}),
          as_field("iCode",{27,27}),
          as_field("x",{31,38}),
          as_field("y",{39,46}),
          as_field("z",{47,54}),
          as_field("occupancy",{55,60}),
          as_field("tempFactor",{61,66}),
          as_field("element",{77,78}),
          as_field("charge",{79,80}),
      }), // ATOM
      as_record("AUTHOR",{
          as_field("continuation",{9,10}),
          as_field("authorList",{11,79}),
      }), // AUTHOR
      as_record("CAVEAT",{
          as_field("continuation",{9,10}),
          as_field("idCode",{12,15}),
          as_field("comment",{20,79}),
      }), // CAVEAT
      as_record("CISPEP",{
          as_field("serNum",{8,10}),
          as_field("pep1",{12,14}),
          as_field("chainID1",{16,16}),
          as_field("seqNum1",{18,21}),
          as_field("icode1",{22,22}),
          as_field("pep2",{26,28}),
          as_field("chainID2",{30,30}),
          as_field("seqNum2",{32,35}),
          as_field("icode2",{36,36}),
          as_field("modNum",{44,46}),
          as_field("measure",{54,59}),
      }), // CISPEP
      as_record("COMPND",{
          as_field("continuation",{8,10}),
          as_field("compound",{11,80}),
      }), // COMPND
      as_record("CONECT",{
          as_field("serial",{7,11,12,16,17,21,22,26,27,31}),
      }), // CONECT
      as_record("CRYST1",{
          as_field("a",{7,15}),
          as_field("b",{16,24}),
          as_field("c",{25,33}),
          as_field("alpha",{34,40}),
          as_field("beta",{41,47}),
          as_field("gamma",{48,54}),
          as_field("sGroup",{56,66}),
          as_field("z",{67,70}),
      }), // CRYST1
      as_record("DBREF",{
          as_field("idCode",{8,11}),
          as_field("chainID",{13,13}),
          as_field("seqBegin",{15,18}),
          as_field("insertBegin",{19,19}),
          as_field("seqEnd",{21,24}),
          as_field("insertEnd",{25,25}),
          as_field("database",{27,32}),
          as_field("dbAccession",{34,41}),
          as_field("dbIdCode",{43,54}),
          as_field("dbseqBegin",{56,60}),
          as_field("idbnsBeg",{61,61}),
          as_field("dbseqEnd",{63,67}),
          as_field("dbinsEnd",{68,68}),
      }), // DBREF
      as_record("DBREF1",{
          as_field("idCode",{8,11}),
          as_field("chainID",{13,13}),
          as_field("seqBegin",{15,18}),
          as_field("insertBegin",{19,19}),
          as_field("seqEnd",{21,24}),
          as_field("insertEnd",{25,25}),
          as_field("database",{27,32}),
          as_field("dbIdCode",{48,67}),
      }), // DBREF1
      as_record("DBREF2",{
          as_field("idCode",{8,11}),
          as_field("chainID",{13,13}),
          as_field("dbAccession",{19,40}),
          as_field("seqBegin",{46,55}),
          as_field("seqEnd",{58,67}),
      }), // DBREF2
      as_record("END",{
      }), // END
      as_record("ENDMDL",{
      }), // ENDMDL
      as_record("EXPDTA",{
          as_field("continuation",{9,10}),
          as_field("technique",{11,79}),
      }), // EXPDTA
      as_record("FORMUL",{
          as_field("compNum",{9,10}),
          as_field("hetID",{13,15}),
          as_field("continuation",{17,18}),
          as_field("asterisk",{19,19}),
          as_field("text",{20,70}),
      }), // FORMUL
      as_record("HEADER",{
          as_field("classification",{11,50}),
          as_field("depDate",{51,59}),
          as_field("idCode",{63,66}),
      }), // HEADER
      as_record("HELIX",{
          as_field("serNum",{8,10}),
          as_field("helixID",{12,14}),
          as_field("initResName",{16,18}),
          as_field("initChainID",{20,20}),
          as_field("initSeqNum",{22,25}),
          as_field("initICode",{26,26}),
          as_field("endResName",{28,30}),
          as_field("endChainID",{32,32}),
          as_field("endSeqNum",{34,37}),
          as_field("endICode",{38,38}),
          as_field("helixClass",{39,40}),
          as_field("comment",{41,70}),
          as_field("length",{72,76}),
      }), // HELIX
      as_record("HET",{
          as_field("hetID",{8,10}),
          as_field("ChainID",{13,13}),
          as_field("seqNum",{14,17}),
          as_field("iCode",{18,18}),
          as_field("numHetAtoms",{21,25}),
          as_field("text",{31,70}),
      }), // HET
      as_record("HETATM",{
          as_field("serial",{7,11}),
          as_field("name",{13,16}),
          as_field("altLoc",{17,17}),
          as_field("resName",{18,20}),
          as_field("chainID",{22,22}),
          as_field("resSeq",{23,26}),
          as_field("iCode",{27,27}),
          as_field("x",{31,38}),
          as_field("y",{39,46}),
          as_field("z",{47,54}),
          as_field("occupancy",{55,60}),
          as_field("tempFactor",{61,66}),
          as_field("element",{77,78}),
          as_field("charge",{79,80}),
      }), // HETATM
      as_record("HETNAM",{
          as_field("continuation",{9,10}),
          as_field("hetID",{12,14}),
          as_field("text",{16,70}),
      }), // HETNAM
      as_record("HETSYN",{
          as_field("continuation",{9,10}),
          as_field("hetID",{12,14}),
          as_field("hetSynonyms",{16,70}),
      }), // HETSYN
      as_record("JRNL",{
          as_field("text",{13,79}),
      }), // JRNL
      as_record("KEYWDS",{
          as_field("continuation",{9,10}),
          as_field("keywds",{11,79}),
      }), // KEYWDS
      as_record("LINK",{
          as_field("name1",{13,16}),
          as_field("altLoc1",{17,17}),
          as_field("resName1",{18,20}),
          as_field("chainID1",{22,22}),
          as_field("resSeq1",{23,26}),
          as_field("iCode1",{27,27}),
          as_field("name2",{43,46}),
          as_field("altLoc2",{47,47}),
          as_field("resName2",{48,50}),
          as_field("chainID2",{52,52}),
          as_field("resSeq2",{53,56}),
          as_field("iCode2",{57,57}),
          as_field("sym1",{60,65}),
          as_field("sym2",{67,72}),
          as_field("Length",{74,78}),
      }), // LINK
      as_record("MASTER",{
          as_field("numRemark",{11,15}),
          as_field("0",{16,20}),
          as_field("numHet",{21,25}),
          as_field("numHelix",{26,30}),
          as_field("numSheet",{31,35}),
          as_field("numTurn",{36,40}),
          as_field("numSite",{41,45}),
          as_field("numXform",{46,50}),
          as_field("numCoord",{51,55}),
          as_field("numTer",{56,60}),
          as_field("numConect",{61,65}),
          as_field("numSeq",{66,70}),
      }), // MASTER
      as_record("MDLTYP",{
          as_field("continuation",{9,10}),
          as_field("comment",{11,80}),
      }), // MDLTYP
      as_record("MODEL",{
          as_field("serial",{11,14}),
      }), // MODEL
      as_record("MODRES",{
          as_field("idCode",{8,11}),
          as_field("resName",{13,15}),
          as_field("chainID",{17,17}),
          as_field("seqNum",{19,22}),
          as_field("iCode",{23,23}),
          as_field("stdRes",{25,27}),
          as_field("comment",{30,70}),
      }), // MODRES
      as_record("MTRIX1",{
          as_field("serial",{8,10}),
          as_field("m[1][1]",{11,20}),
          as_field("m[1][2]",{21,30}),
          as_field("m[1][3]",{31,40}),
          as_field("v[1]",{46,55}),
          as_field("iGiven",{60,60}),
      }), // MTRIX1
      as_record("MTRIX2",{
          as_field("serial",{8,10}),
          as_field("m[2][1]",{11,20}),
          as_field("m[2][2]",{21,30}),
          as_field("m[2][3]",{31,40}),
          as_field("v[2]",{46,55}),
          as_field("iGiven",{60,60}),
      }), // MTRIX2
      as_record("MTRIX3",{
          as_field("serial",{8,10}),
          as_field("m[3][1]",{11,20}),
          as_field("m[3][2]",{21,30}),
          as_field("m[3][3]",{31,40}),
          as_field("v[3]",{46,55}),
          as_field("iGiven",{60,60}),
      }), // MTRIX3
      as_record("NUMMDL",{
          as_field("modelNumber",{11,14}),
      }), // NUMMDL
      as_record("OBSLTE",{
          as_field("continuation",{9,10}),
          as_field("repDate",{12,20}),
          as_field("idCode",{22,25}),
          as_field("rIdCode",{32,35,37,40,42,45,47,50,52,55,57,60,62,65,67,70,72,75}),
      }), // OBSLTE
      as_record("ORIGX1",{
          as_field("o[1][1]",{11,20}),
          as_field("o[1][2]",{21,30}),
          as_field("o[1][3]",{31,40}),
          as_field("t[1]",{46,55}),
      }), // ORIGX1
      as_record("ORIGX2",{
          as_field("o[2][1]",{11,20}),
          as_field("o[2][2]",{21,30}),
          as_field("o[2][3]",{31,40}),
          as_field("t[2]",{46,55}),
      }), // ORIGX2
      as_record("ORIGX3",{
          as_field("o[3][1]",{11,20}),
          as_field("o[3][2]",{21,30}),
          as_field("o[3][3]",{31,40}),
          as_field("t[3]",{46,55}),
      }), // ORIGX3
      as_record("REMARK",{
          as_field("remarkNum",{8,10}),
          as_field("empty",{12,79}),
      }), // REMARK
      as_record("REVDAT",{
          as_field("modNum",{8,10}),
          as_field("continuation",{11,12}),
          as_field("modDate",{14,22}),
          as_field("modId",{24,27}),
          as_field("modType",{32,32}),
          as_field("record",{40,45,47,52,54,59,61,66}),
      }), // REVDAT
      as_record("SCALE1",{
          as_field("s[1][1]",{11,20}),
          as_field("s[1][2]",{21,30}),
          as_field("s[1][3]",{31,40}),
          as_field("u[1]",{46,55}),
      }), // SCALE1
      as_record("SCALE2",{
          as_field("s[2][1]",{11,20}),
          as_field("s[2][2]",{21,30}),
          as_field("s[2][3]",{31,40}),
          as_field("u[2]",{46,55}),
      }), // SCALE2
      as_record("SCALE3",{
          as_field("s[3][1]",{11,20}),
          as_field("s[3][2]",{21,30}),
          as_field("s[3][3]",{31,40}),
          as_field("u[3]",{46,55}),
      }), // SCALE3
      as_record("SEQADV",{
          as_field("idCode",{8,11}),
          as_field("resName",{13,15}),
          as_field("chainID",{17,17}),
          as_field("seqNum",{19,22}),
          as_field("iCode",{23,23}),
          as_field("database",{25,28}),
          as_field("dbAccession",{30,38}),
          as_field("dbRes",{40,42}),
          as_field("dbSeq",{44,48}),
          as_field("conflict",{50,70}),
      }), // SEQADV
      as_record("SEQRES",{
          as_field("serNum",{8,10}),
          as_field("chainID",{12,12}),
          as_field("numRes",{14,17}),
          as_field("resName",{20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70}),
      }), // SEQRES
      as_record("SHEET",{
          as_field("strand",{8,10}),
          as_field("sheetID",{12,14}),
          as_field("numStrands",{15,16}),
          as_field("initResName",{18,20}),
          as_field("initChainID",{22,22}),
          as_field("initSeqNum",{23,26}),
          as_field("initICode",{27,27}),
          as_field("endResName",{29,31}),
          as_field("endChainID",{33,33}),
          as_field("endSeqNum",{34,37}),
          as_field("endICode",{38,38}),
          as_field("sense",{39,40}),
          as_field("curAtom",{42,45}),
          as_field("curResName",{46,48}),
          as_field("curChainId",{50,50}),
          as_field("curResSeq",{51,54}),
          as_field("curICode",{55,55}),
          as_field("prevAtom",{57,60}),
          as_field("prevResName",{61,63}),
          as_field("prevChainId",{65,65}),
          as_field("prevResSeq",{66,69}),
          as_field("prevICode",{70,70}),
      }), // SHEET
      as_record("SIGATM",{
          as_field("serial",{7,11}),
          as_field("name",{13,16}),
          as_field("altLoc",{17,17}),
          as_field("resName",{18,20}),
          as_field("chainID",{22,22}),
          as_field("resSeq",{23,26}),
          as_field("iCode",{27,27}),
          as_field("sigX",{31,38}),
          as_field("sigY",{39,46}),
          as_field("sigZ",{47,54}),
          as_field("sigOcc",{55,60}),
          as_field("sigTemp",{61,66}),
          as_field("element",{77,78}),
          as_field("charge",{79,80}),
      }), // SIGATM
      as_record("SIGUIJ",{
          as_field("serial",{7,11}),
          as_field("name",{13,16}),
          as_field("altLoc",{17,17}),
          as_field("resName",{18,20}),
          as_field("chainID",{22,22}),
          as_field("resSeq",{23,26}),
          as_field("iCode",{27,27}),
          as_field("sig[0][0]",{29,35}),
          as_field("sig[1][1]",{36,42}),
          as_field("sig[2][2]",{43,49}),
          as_field("sig[0][1]",{50,56}),
          as_field("sig[0][2]",{57,63}),
          as_field("sig[1][2]",{64,70}),
          as_field("element",{77,78}),
          as_field("charge",{79,80}),
      }), // SIGUIJ
      as_record("SITE",{
          as_field("seqNum",{8,10}),
          as_field("siteID",{12,14}),
          as_field("numRes",{16,17}),
          as_field("resName1",{19,21}),
          as_field("chainID1",{23,23}),
          as_field("seq1",{24,27}),
          as_field("iCode1",{28,28}),
          as_field("resName2",{30,32}),
          as_field("chainID2",{34,34}),
          as_field("seq2",{35,38}),
          as_field("iCode2",{39,39}),
          as_field("resName3",{41,43}),
          as_field("chainID3",{45,45}),
          as_field("seq3",{46,49}),
          as_field("iCode3",{50,50}),
          as_field("resName4",{52,54}),
          as_field("chainID4",{56,56}),
          as_field("seq4",{57,60}),
          as_field("iCode4",{61,61}),
      }), // SITE
      as_record("SOURCE",{
          as_field("continuation",{8,10}),
          as_field("srcName",{11,79}),
      }), // SOURCE
      as_record("SPLIT",{
          as_field("continuation",{9,10}),
          as_field("idCode",{12,15,17,20,22,25,27,30,32,35,37,40,42,45,47,50,52,55,57,60,62,65,67,70,72,75,77,80}),
      }), // SPLIT
      as_record("SPRSDE",{
          as_field("continuation",{9,10}),
          as_field("sprsdeDate",{12,20}),
          as_field("idCode",{22,25}),
          as_field("sIdCode",{32,35,37,40,42,45,47,50,52,55,57,60,62,65,67,70,72,75}),
      }), // SPRSDE
      as_record("SSBOND",{
          as_field("serNum",{8,10}),
          as_field("CYS",{26,28}),
          as_field("chainID1",{16,16}),
          as_field("seqNum1",{18,21}),
          as_field("icode1",{22,22}),
          as_field("chainID2",{30,30}),
          as_field("seqNum2",{32,35}),
          as_field("icode2",{36,36}),
          as_field("sym1",{60,65}),
          as_field("sym2",{67,72}),
          as_field("Length",{74,78}),
      }), // SSBOND
      as_record("TER",{
          as_field("serial",{7,11}),
          as_field("resName",{18,20}),
          as_field("chainID",{22,22}),
          as_field("resSeq",{23,26}),
          as_field("iCode",{27,27}),
      }), // TER
      as_record("TITLE",{
          as_field("continuation",{9,10}),
          as_field("title",{11,80}),
      }), // TITLE
  };
  return result;
}
