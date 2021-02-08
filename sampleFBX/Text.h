#pragma once

class Material;

class Text {
private:
	char m_szText[64];		//!< �������
	float m_fontSize[2];	//!< �t�H���g�T�C�Y
	Material* m_material;	//!< �}�e���A��

public:
	Text();
	~Text();
	void SetText(const char *fmt, ...);
	void SetFontSize(float size);
	void SetFontSize(float w, float h);
	void Bind();
};

// EOF