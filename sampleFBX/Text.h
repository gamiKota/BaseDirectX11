#pragma once

class Text {
public:
	char m_szText[64];	//!< �������

	Text() {
		m_szText[0] = '\0';
	}
	void Init();
	void Set(const char *fmt, ...);
	void Bind();
};

// EOF