import java.io.*;
import java.util.*;

class SLIDER {
	public static final int SIZE = 4;
	int Board[][] = new int[SIZE][SIZE];
	Scanner s  = new Scanner(System.in);
	int ec = 3, er = 3;
	int er1 = 3, ec1 = 3;
	public void initBoard(){
		Board[0] = new int[]{1, 2, 3, 4};
		Board[1] = new int[]{5, 6, 7, 8};
		Board[2] = new int[]{9, 10, 12, 15};
		Board[3] = new int[]{13, 14, 11, 0};
	}

	public void printboard(){
		for(int i = 0; i < SIZE; i++){
			for(int j = 0; j < SIZE; j++){
				if (Board[i][j] == 0)
					System.out.print(" \t");
				else
					System.out.print(Board[i][j] + "\t");
			}
			System.out.println();
		}
	}
	public void swap(int er , int ec, int er1, int ec1){
		int temp = Board[er][ec];
		Board[er][ec] = Board[er1][ec1];
		Board[er1][ec1] = temp;
	}
	public boolean checkswap(int a) {
		// Check DOWN
		if (er + 1 < SIZE && Board[er + 1][ec] == a) {
			er1 = er + 1;
			ec1 = ec;
			return true;
		}
		// Check UP
		if (er - 1 >= 0 && Board[er - 1][ec] == a) {
			er1 = er - 1;
			ec1 = ec;
			return true;
		}
		// Check RIGHT
		if (ec + 1 < SIZE && Board[er][ec + 1] == a) {
			er1 = er;
			ec1 = ec + 1;
			return true;
		}
		// Check LEFT
		if (ec - 1 >= 0 && Board[er][ec - 1] == a) {
			er1 = er;
			ec1 = ec - 1;
			return true;
		}
		return false;
	}
	
	public Boolean checkwin(){
		int count = 1;
		for(int i = 0; i < SIZE; i++){
			for(int j = 0; j < SIZE; j++){
				// Check if each tile is in the correct position (excluding the last tile)
				if (i == SIZE - 1 && j == SIZE - 1) {
					// The last tile should be 0
					if (Board[i][j] != 0) {
						return false;
					}
				} else {
					// Check if the current tile matches the expected value
					if (Board[i][j] != count) {
						return false;
					}
					count++;
				}
			}
		}
		return true;
	}
	
	public void option(){
		int option;
		initBoard();
		while (true) {
			printboard();
			if(checkwin()){
				System.out.println("Congrats on solving!!");
				System.exit(0);
			}
			System.out.println("Enter the number to slide the space. Enter -1 to exit");
			System.out.print("Enter number to swap: ");
			option = s.nextInt();
			if(option == -1)
				System.exit(0);
			else{
				if(checkswap(option)){
					swap(er,ec,er1,ec1);
					er = er1;
					ec = ec1;
				}
				else {
					System.out.println("Unable to swap!!");
				}
			}

		}
	}
}

class NumberSlidingGame {
	public static void main(String args[]){
		SLIDER sli = new SLIDER();
		sli.option();
	}

}
