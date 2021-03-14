#ifndef TAXPAYER_H
#define TAXPAYER_H

class Taxpayer{
	private:
		int RID;
		char * firstName;
		char * lastName;
		int numOfDepd;
		int income;
		int posCode;

	public:
		Taxpayer();
		~Taxpayer();
		int getRid();
		void setRid(int arg);
		char * getFirstName();
		void setFirstName(char * arg);
		char * getLastName();
		void setLastName(char * arg);
		int getNumOfDepd();
		void setNumOfDepd(int arg);
		int getIncome();
		void setIncome(int arg);
		int getPosCode();
		void setPosCode(int arg);
};

#endif