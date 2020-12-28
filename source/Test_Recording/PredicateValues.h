#pragma once
//predicate IDs for the relative x position
enum class RelX : int {
	BEHIND = 102030,
	CLOSE_BEHIND = 102040,
	SAME_HEIGHT = 102050,
	CLOSE_IN_FRONT = 102060,
	IN_FRONT = 102070
};

static std::string to_string(RelX relX) {
	switch(relX) {
	case RelX::BEHIND:
		return "BEHIND";
	case RelX::CLOSE_BEHIND:
		return "CLOSE_BEHIND";
	case RelX::SAME_HEIGHT:
		return "SAME_HEIGHT";
	case RelX::CLOSE_IN_FRONT:
		return "CLOSE_IN_FRONT";
	case RelX::IN_FRONT:
		return "IN_FRONT";
	default:
		return "UNDEFINED";
	}
}

//predicate IDs for the relative velocity
enum class RelV : int {
	SLOWER = 103040,
	SAME = 103050,
	FASTER = 103060
};

static std::string to_string(RelV relV) {
	switch(relV) {
	case RelV::SLOWER:
		return "SLOWER";
	case RelV::SAME:
		return "SAME";
	case RelV::FASTER:
		return "FASTER";
	default:
		return "UNDEFINED";
	}
}

//Acceleration used in the VUT
enum class Acc : int {
	BRAKE = 105040,
	NONE = 105050,
	ACCELERATE = 105060
};

static std::string to_string(Acc acc) {
	switch(acc) {
	case Acc::BRAKE:
		return "BRAKE";
	case Acc::NONE:
		return "NONE";
	case Acc::ACCELERATE:
		return "ACCELERATE";
	default:
		return "UNDEFINED";
	}
}

//Lane change used in the VUT
enum class LnChg : int {
	RIGHT = 106040,
	NONE = 106050,
	LEFT = 106060
};

static std::string to_string(LnChg lnChg) {
	switch(lnChg) {
	case LnChg::RIGHT:
		return "RIGHT";
	case LnChg::NONE:
		return "NONE";
	case LnChg::LEFT:
		return "LEFT";
	default:
		return "UNDEFINED";
	}
}