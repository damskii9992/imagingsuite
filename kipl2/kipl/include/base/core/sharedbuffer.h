//<LICENCE>

#ifndef SHAREDBUFFER_H_
#define SHAREDBUFFER_H_

#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <xmmintrin.h>

#include "../KiplException.h"

using namespace std;
namespace kipl { namespace base { namespace core {


template <typename T>
class buffer {
	/// \brief Internal class that manages the memory buffer
	class cref {
		public:
		/// \brief C'Tor that allocates the buffer
		cref(size_t N) : cnt(1), m_nData(N) {_Allocate(N);}
		/// \brief reference counter
		int cnt;
		/// \brief Pointer to the allocated buffer
		T *data;
		/// \brief D'tor deallocated the buffer
		~cref() { _mm_free(m_pRawPointer); }
		//~cref() { delete [] m_pRawPointer; }
		/// \returns The size of the allocated buffer 
		size_t Size() { return m_nData; }
        /// \returns True is the buffer pointer is adjusted to fit better into 32 byte blocks
		bool AdjustedPointer() { return m_pRawPointer!=data; }
		
		private:
			/// \brief Number of elements in the buffer
			size_t m_nData;
			/// \brief Pointer to the allocated buffer
			char *m_pRawPointer;
			/// \brief Does the allocation and adjusts the data pointer to the beginning of the next 32 block
			void _Allocate(size_t N) {
				m_pRawPointer=reinterpret_cast<char *>(_mm_malloc((N+16)*sizeof(T),32));               

                if (m_pRawPointer==nullptr) {
					std::ostringstream msg;
					msg<<"Failed to allocate "<<(N+16)*sizeof(T)<<" bytes";
                    throw kipl::base::KiplException(msg.str(),std::string(__FILE__),size_t(__LINE__));
				}
				data=reinterpret_cast<T*>(m_pRawPointer);
			}
	};

	/// \brief Pointer to the current data buffer 
	cref *m_cref;

	public:
		///\brief C'Tor that creates a new data buffer with the instance itself as only owner 
		buffer(size_t N) : m_cref(new cref(N)) {}
		///\brief Copy c'tor. Manages the refence counting (cheap execution)
		buffer(const buffer &a) {
			m_cref=a.m_cref; 
            ++m_cref->cnt;
		}
		
		///\brief Assignment operator. Manages reference counting (cheap execution)
		buffer & operator=(const buffer &a) {
			if (!(--m_cref->cnt)) 
				delete m_cref;
			m_cref=a.m_cref; 
            ++m_cref->cnt;
			
			return *this;
		}

		buffer & Resize(size_t N) {
			if (N!=this->m_cref->Size()) {
                if (!(--m_cref->cnt))
                    delete m_cref;

				m_cref=new cref(N);
			}

			return *this;
		}
		/// \returns The size of the data buffer
		size_t Size() const { return m_cref->Size(); }

		/// \brief D'tor removes the buffer only if the current instance was the last to refer to it. 
		~buffer() {
			if (!(--m_cref->cnt)) 
				delete m_cref;
		}
		
		T * GetDataPtr() {return m_cref->data;}
        const T * GetDataPtr() const {return m_cref->data;}

		///\brief Performs a deep copy of the data buffer. An new buffer is created. 
		void Clone() {
			if (this->m_cref->cnt>1) {
				cref *tmp=this->m_cref;
				Clone(tmp);
			}
		}
		/// \returns The number of sharing objects refering to the same buffer
		int References() { return m_cref->cnt; }
		/// \brief Determines if the current instance shares buffer with the other
		/// \param Buffer instance to compare 
		/// \returns true if the instances share the same internal buffer
		bool operator==(const buffer &b) { return b.m_cref==m_cref; }
		/// \brief Determines if the current instance shares buffer with the other
		/// \param Buffer instance to compare 
		/// \returns false if the instances share the same internal buffer
		bool operator!=(const buffer &b) { return b.m_cref!=m_cref; }
		/// \brief Access operator. Gives access to the elements in the internal buffer
		/// \param i index in the buffer.
		T & operator[](const size_t index) { return m_cref->data[index];}
		T operator[](const size_t index) const { return m_cref->data[index];}
	private:
		/// \brief The Actual clone method that performs that cloning operation
		/// \param c internal buffer to clone
		void Clone(cref *c) {
			this->m_cref=new cref(c->Size());
			memcpy(this->m_cref->data,c->data,c->Size()*sizeof(T));
			int n=--c->cnt;
			if (n==0) {
				delete c;
			}	
		}
		
};

}}}
#endif /*SHAREDBUFFER_H_*/
