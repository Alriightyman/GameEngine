
#pragma once
#include <d3d11.h>
///
/// \brief Wrapper class for cbuffers that handles creation and updating
///  for a fixed type specified by the template parameter T.
///
template<typename T>
class ConstantBuffer
{
private:
	ID3D11Buffer* m_Buffer;
	bool m_Initialized;
public:
	/// Public structure instance mirroring the data stored in the constant buffer.
	T Data;

public:
	/// Returns the d3d11 buffer this class wraps. 
	ID3D11Buffer* Buffer() const { return m_Buffer; }

public:
	ConstantBuffer() : m_Initialized(false), m_Buffer(0)
	{
	}


	~ConstantBuffer() { if(m_Buffer) { m_Buffer->Release(); m_Buffer = 0; } }
	/// Initializes the constant buffer.
	void Initialize(ID3D11Device* device)
	{
		// Make the constant buffer multiple of 16 bytes
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		device->CreateBuffer(&desc,0,&m_Buffer);

		m_Initialized = true;
	}

	///
	/// Copies the system memory constant buffer data to the GPU constant buffer.
	/// This call should be made as infrequently as possible.
	///
	void ApplyChanges(ID3D11DeviceContext* dc)
	{
		_ASSERT(m_Initialized);

		D3D11_MAPPED_SUBRESOURCE mappedResorce;
		dc->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResorce);
		memcpy(mappedResorce.pData,&Data, sizeof(T));
		dc->Unmap(m_Buffer,0);
	}
};

