#ifndef  EXCEPTIONS_H
#define EXCEPTIONS_H
#include<string>

class Exception{};

class TurnException : public Exception{};

class MoveException : public Exception{};

class RangeException : public Exception{};

class FieldIsBusyException : public Exception{};

class IOException : public Exception{
	std::string reason;
	public:
		IOException(){
			reason="BLAD WEJSCIA/WYJSCIA";
		}

		IOException(std::string reason){
			this->reason=reason;
		}
	std::string getReason(){
		return reason;
	}
};

class TrackNotEmptyException : public Exception{};

class Check{
    std::string reason;
    public:
        Check(){
            this->reason="SZACH";
        }

        Check(std::string reason){
            this->reason=reason;
        }

        std::string getReason(){
            return reason;
        }
};

class GameOver : public Exception{};

class NoPrevSave : public Exception{};

class NullField : public Exception{};
#endif
