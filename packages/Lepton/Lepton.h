#ifndef LEPTON_H
#define LEPTON_H 1

#include "TLorentzVector.h"


class Lepton{
	public:
		Lepton(){
      p = TLorentzVector(0,0,0,0);
      charge = 0;
      type = -1;
      isElec = 0; isMuon = 0;
    };
		//Lepton(const Lepton &l): p(l.p), charge(l.charge), type(l.type), index(l.index){ };
		Lepton(TLorentzVector vec, Int_t ch = 0, Int_t ty = -1){
			p = vec;
			charge = ch;
			type = ty;

			if(ty == 0){ isMuon = 1; isElec = 0;}
			if(ty == 1){ isMuon = 0; isElec = 1;}
		}
		~Lepton(){};
		TLorentzVector p;
		Int_t charge;
		Int_t type;
    Bool_t isElec;
    Bool_t isMuon;
		Float_t GetSF(Int_t sys);
		void SetSF(Float_t val);
		void SetSFerr(Float_t val);

	protected:
		Float_t SF;
		Float_t SFerr;
};

#endif
