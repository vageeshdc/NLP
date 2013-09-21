all:	phrase
phrase:	gt
	g++ -std=c++0x -g -o phrase phraseWorker.cpp hash_test.cpp -lsqlite3 -lm -I.
gt:
	g++ -std=c++0x -g -o gtsmooth GT_smoothing.cpp -lm -I.
clean:
	rm *.o ./phrase
