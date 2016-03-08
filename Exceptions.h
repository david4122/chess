#ifndef  EXCEPTIONS_H
#define EXCEPTIONS_H
class TurnException{};

class MoveException{};

class RangeException{};

class FieldIsBusyException{};

class IOException{
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

class TrackNotEmptyException{};

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

class GameOver{};

class NoPrevSave{};

class NullField{};
#endif
