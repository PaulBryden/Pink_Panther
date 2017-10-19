#ifndef NODEAP_H
#define NODEAP_H

class NodeAP {

private:
	int public_WirelessModule_m_WirelessMod;
	int public_Coords_m_Coordinates;

public:
	NodeAP(int string_p_AccessPoint, int WirelessModule_p_WirelessMod, int Coords_m_Coords);

	NodeAP(int string_p_AccessPoint, int WirelessModule_p_WirelessMod, int int_p_X, int int_p_Y);

	NodeAP(int NodeAP_m_NodeAP);
};

#endif
