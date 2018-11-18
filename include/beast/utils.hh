#pragma once

#include "threadPool.hh"
#include "api.hh"

#include <cmath>
#include <string>

/// Lots of useful utilities

namespace Beast
{
	namespace Util
	{
		struct CTextFile
		{
			char *data;
			size_t length;
		};
		
		BEAST_API extern ThreadPool threadPool;

		/// Get a textual version of the given OpenGL error code, must be called from the thread that OpenGL is running on
		BEAST_API std::string getGLErrorMessage(int32_t errorCode);

		/// Get the current working directory
		BEAST_API std::string getCWD();

		/// Get the contents of a text file in an std::string
		BEAST_API std::string readTextFile(std::string const &filePath);
		BEAST_API CTextFile readTextFileC(std::string const &filePath);

		/// Create the specified directory if it doesn't exist
		BEAST_API void createDirectory(std::string const &folderPath);

		/// Pull the current buffer from OpenGL and write it as a PNG image to disk at the given location, must be called from the thread that OpenGL is running on
		BEAST_API void writeScreenshot(std::string const &folderPath, uint32_t const &width, uint32_t const &height);

		/// Merge 2 or more vectors' contents by appending them together
		template <typename T> std::vector<T> mergeVectors(std::initializer_list<std::vector<T>> const &vectors)
		{
			std::vector<T> out;
			for(auto const &vec : vectors)
			{
				out.insert(out.end(), vec.begin(), vec.end());
			}
			return out;
		}

		/// Get the Adler32 checksum of the given data
		BEAST_API uint32_t getAdler32Checksum(unsigned char const *data, size_t length);
		
		namespace StringTools
		{
			/// Check if the given string contains the given token
			BEAST_API bool contains(std::string const &input, std::string const &searchToken);
			
			/// Check if the given string starts with the given token
			BEAST_API bool startsWith(std::string const &input, std::string const &searchToken);
			
			/// Check if the given string ends with the given token
			BEAST_API bool endsWith(std::string const &input, std::string const &searchToken);
			
			/// Cut characters off either end of a string
			BEAST_API std::string subString(std::string const &input, uint32_t const &distFromStart, uint32_t const &distFromEnd);
			
			/// Trim leading and trailing whitespace
			BEAST_API std::string trim(std::string const &input);
			
			/// Remove all occurences of the given character from the input string
			BEAST_API std::string remove(std::string const &input, char const &remove);
			
			/// Remove all occurences of any of the given characters from the input string
			BEAST_API std::string removeMult(std::string const &input, std::vector<char> const &remove);
			
			/// Split a string on the given character
			BEAST_API std::vector<std::string> split(std::string const &input, char const &splitOn, bool const &removeMatches);
			
			/// Split a string on any of the given characters
			BEAST_API std::vector<std::string> splitMult(std::string const &input, std::vector<char> const &splitOn, bool const &removeMatches);
			
			/// Split a string on the given word
			BEAST_API std::vector<std::string> splitSeq(std::string const &input, std::string const &splitOn);
			
			/// Split a string on any of the given words
			BEAST_API std::vector<std::string> splitSeqMult(std::string const &input, std::vector<std::string> const &splitOn);
		}
		
		struct FileBrowser
		{
			std::string current;
			
			inline void sanitize()
			{
				std::string temp;
				temp.reserve(this->current.size());
				for(char const &c : this->current) temp.push_back(c == '\\' ? '/' : c);
				this->current = temp;
			}
			
			inline FileBrowser& setToCWD()
			{
				this->current = getCWD();
				this->sanitize();
				return *this;
			}
			
			inline FileBrowser& setDirectory(std::string const &dir)
			{
				this->current = dir;
				this->sanitize();
				return *this;
			}
			
			inline FileBrowser& forward(std::string const &relativeDir)
			{
				this->current += "/" + relativeDir;
				this->sanitize();
				return *this;
			}
			
			inline FileBrowser& back()
			{
				this->current = StringTools::subString(this->current, 0, static_cast<uint32_t>(this->current.length() - this->current.find_last_of('/')));
				this->sanitize();
				return *this;
			}
		};
	}
}
