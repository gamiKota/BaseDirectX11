#include "Geometory.h"
#include "DrawBuffer.h"

#define LINE_NUM (1000)
#define LINE_GEOMETORY_NUM (LINE_NUM * 2)

struct GeometoryVertex
{
	float pos[3];
	float color[4];
	float uv[2];
	float normal[3];
};

UINT g_lineIndex;
GeometoryVertex g_lineData[LINE_GEOMETORY_NUM];
DrawBuffer g_lines;
DrawBuffer g_polygon;
DrawBuffer g_cube;


HRESULT InitGeometory()
{
	HRESULT hr;
	const float d = 0.5f;

	//--- Lines
	g_lineIndex = 0;
	hr = g_lines.CreateVertexBuffer(g_lineData, sizeof(GeometoryVertex), LINE_GEOMETORY_NUM, true);
	if (FAILED(hr)) { return hr; }

	//--- Polygon
	GeometoryVertex vtxPolygon[] = {
		{ {-d, d, 0}, {1,1,1,1}, {0,0}, {0,0,-1} },
		{ { d, d, 0}, {1,1,1,1}, {1,0}, {0,0,-1} },
		{ {-d,-d, 0}, {1,1,1,1}, {0,1}, {0,0,-1} },
		{ { d,-d, 0}, {1,1,1,1}, {1,1}, {0,0,-1} },
	};
	hr = g_polygon.CreateVertexBuffer(vtxPolygon, sizeof(GeometoryVertex), _countof(vtxPolygon));
	if (FAILED(hr)) { return hr; }

	//--- Cube
	GeometoryVertex vtxCube[] = {
		// éËëOÇÃñ 
		{ {-d, d,-d}, {1,1,1,1}, {0,0}, {0,0,-1} },
		{ { d, d,-d}, {1,1,1,1}, {1,0}, {0,0,-1} },
		{ {-d,-d,-d}, {1,1,1,1}, {0,1}, {0,0,-1} },
		{ { d, d,-d}, {1,1,1,1}, {1,0}, {0,0,-1} },
		{ { d,-d,-d}, {1,1,1,1}, {1,1}, {0,0,-1} },
		{ {-d,-d,-d}, {1,1,1,1}, {0,1}, {0,0,-1} },
		// âEÇÃñ 
		{ { d, d,-d}, {1,1,1,1}, {0,0}, {1,0,0} },
		{ { d, d, d}, {1,1,1,1}, {1,0}, {1,0,0} },
		{ { d,-d,-d}, {1,1,1,1}, {0,1}, {1,0,0} },
		{ { d, d, d}, {1,1,1,1}, {1,0}, {1,0,0} },
		{ { d,-d, d}, {1,1,1,1}, {1,1}, {1,0,0} },
		{ { d,-d,-d}, {1,1,1,1}, {0,1}, {1,0,0} },
		// å„ÇÎÇÃñ 
		{ { d, d, d}, {1,1,1,1}, {0,0}, {0,0,1} },
		{ {-d, d, d}, {1,1,1,1}, {1,0}, {0,0,1} },
		{ { d,-d, d}, {1,1,1,1}, {0,1}, {0,0,1} },
		{ {-d, d, d}, {1,1,1,1}, {1,0}, {0,0,1} },
		{ {-d,-d, d}, {1,1,1,1}, {1,1}, {0,0,1} },
		{ { d,-d, d}, {1,1,1,1}, {0,1}, {0,0,1} },
		// ç∂ÇÃñ 
		{ {-d, d, d}, {1,1,1,1}, {0,0}, {-1,0,0} },
		{ {-d, d,-d}, {1,1,1,1}, {1,0}, {-1,0,0} },
		{ {-d,-d, d}, {1,1,1,1}, {0,1}, {-1,0,0} },
		{ {-d, d,-d}, {1,1,1,1}, {1,0}, {-1,0,0} },
		{ {-d,-d,-d}, {1,1,1,1}, {1,1}, {-1,0,0} },
		{ {-d,-d, d}, {1,1,1,1}, {0,1}, {-1,0,0} },
		// è„ÇÃñ 
		{ {-d, d, d}, {1,1,1,1}, {0,0}, {0,1,0} },
		{ { d, d, d}, {1,1,1,1}, {1,0}, {0,1,0} },
		{ {-d, d,-d}, {1,1,1,1}, {0,1}, {0,1,0} },
		{ { d, d, d}, {1,1,1,1}, {1,0}, {0,1,0} },
		{ { d, d,-d}, {1,1,1,1}, {1,1}, {0,1,0} },
		{ {-d, d,-d}, {1,1,1,1}, {0,1}, {0,1,0} },
		// â∫ÇÃñ 
		{ {-d,-d,-d}, {1,1,1,1}, {0,0}, {0,-1,0} },
		{ { d,-d,-d}, {1,1,1,1}, {1,0}, {0,-1,0} },
		{ {-d,-d, d}, {1,1,1,1}, {0,1}, {0,-1,0} },
		{ { d,-d,-d}, {1,1,1,1}, {1,0}, {0,-1,0} },
		{ { d,-d, d}, {1,1,1,1}, {1,1}, {0,-1,0} },
		{ {-d,-d, d}, {1,1,1,1}, {0,1}, {0,-1,0} },
	};
	hr = g_cube.CreateVertexBuffer(vtxCube, sizeof(GeometoryVertex), _countof(vtxCube));
	if (FAILED(hr)) { return hr; }

	return hr;
}

void AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4 color)
{
	if (g_lineIndex < LINE_NUM)
	{
		g_lineData[g_lineIndex].pos[0] = start.x;
		g_lineData[g_lineIndex].pos[1] = start.y;
		g_lineData[g_lineIndex].pos[2] = start.z;
		g_lineData[g_lineIndex].color[0] = color.x;
		g_lineData[g_lineIndex].color[1] = color.y;
		g_lineData[g_lineIndex].color[2] = color.z;
		g_lineData[g_lineIndex].color[3] = color.w;
		++g_lineIndex;
		g_lineData[g_lineIndex].pos[0] = end.x;
		g_lineData[g_lineIndex].pos[1] = end.y;
		g_lineData[g_lineIndex].pos[2] = end.z;
		g_lineData[g_lineIndex].color[0] = color.x;
		g_lineData[g_lineIndex].color[1] = color.y;
		g_lineData[g_lineIndex].color[2] = color.z;
		g_lineData[g_lineIndex].color[3] = color.w;
		++g_lineIndex;
	}
}
void DrawLines()
{
	g_lines.Write(g_lineData);
	g_lines.Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	g_lineIndex = 0;
}
void DrawPolygon()
{
	g_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
void DrawCube()
{
	g_cube.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}