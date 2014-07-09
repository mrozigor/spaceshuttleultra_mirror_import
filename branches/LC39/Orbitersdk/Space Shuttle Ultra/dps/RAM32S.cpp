#include "RAM32.h"
#include <limits>

namespace dps {
	
	RAM32S::RAM32S()
	{

	}

	RAM32S::~RAM32S()
	{
	}

	word32 RAM32S::GetDWord(unsigned long ulAddress) const
	{
		unsigned long w_address = ulAddress >> 2;
		if( w_address < GetNumberOfWords())
			return m_memory[w_address].dword;
		else
			return word32(0xAAAAAAAA);
	}

	word16 RAM32S::GetWord(unsigned long ulAddress) const
	{
		unsigned long w_address = ulAddress >> 2;
		unsigned int word_no = (ulAddress >> 1) & 0x1;

		if(w_address < GetNumberOfWords())
			return m_memory[w_address].word[word_no];
		else
			return word16(0xAAAA);
	}

	float32 RAM32S::GetFloat(unsigned long ulAddress) const
	{
		unsigned long w_address = ulAddress >> 2;
		if( w_address < GetNumberOfWords())
			return m_memory[w_address].fl32;
		else
			return std::numeric_limits<float>::signaling_NaN();
	}

	unsigned long RAM32S::GetNumberOfWords() const {
		return AP101S_MEMORY_SIZE;
	};

	void RAM32S::SetDWord(unsigned long ulAddress, word32 dword)
	{
		unsigned long w_address = ulAddress >> 2;
		if( w_address < GetNumberOfWords())
			m_memory[w_address].dword = dword;
	}

	void RAM32S::SetWord(unsigned long ulAddress, word16 word)
	{
		unsigned long w_address = ulAddress >> 2;
		unsigned int word_no = (ulAddress >> 1) & 0x1;
		if( w_address < GetNumberOfWords())
			m_memory[w_address].word[word_no] = word;
	}

	void RAM32S::SetFloat(unsigned long ulAddress, float32 fword)
	{
		unsigned long w_address = ulAddress >> 2;
		if( w_address < GetNumberOfWords())
			m_memory[w_address].fl32 = fword;
	}

};