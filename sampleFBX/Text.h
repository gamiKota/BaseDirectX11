#pragma once

class Material;

class Text {
private:
	char m_szText[64];		//!< 文字情報
	float m_fontSize[2];	//!< フォントサイズ
	Material* m_material;	//!< マテリアル

public:
	Text();
	~Text();
	void SetText(const char *fmt, ...);
	void SetFontSize(float size);
	void SetFontSize(float w, float h);
	void Bind();
};

// EOF