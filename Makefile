CXX = g++
CPPFLAGS = -Wall -g -std=c++17
PROGS = huffman

all: $(PROGS)

huffman: huffman.o CompressedReader.o CompressedWriter.o UncompressedReader.o TreeNode.o Bits.o
	$(CXX) $(CPPFLAGS) -o $@ $^

huffman.o: huffman.cpp CompressedReader.h CompressedWriter.h UncompressedReader.h TreeNode.h Bits.h
	$(CXX) $(CPPFLAGS) -c $<

CompressedReader.o: CompressedReader.cpp CompressedWriter.h CompressedReader.h
	$(CXX) $(CPPFLAGS) -c $<

CompressedWriter.o: CompressedWriter.cpp TreeNode.h CompressedWriter.h
	$(CXX) $(CPPFLAGS) -c $<

UncompressedReader.o: UncompressedReader.cpp TreeNode.h UncompressedReader.h
	$(CXX) $(CPPFLAGS) -c $<

TreeNode.o: TreeNode.cpp Bits.h TreeNode.h
	$(CXX) $(CPPFLAGS) -c $<

Bits.o: Bits.cpp Bits.h
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -rf *.o *~ *.dSYM $(PROGS)
