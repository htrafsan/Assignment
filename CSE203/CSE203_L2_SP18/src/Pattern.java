/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author official_rafsan
 */
import java.util.Scanner;
public class Pattern {

	/**
	 * 
	 */
	public Pattern() {
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Enter Total Line Number");
		Scanner scan = new Scanner(System.in);
		int n = scan.nextInt();
		Pattern(n);

	}
	
	public static void Pattern(int n) {
		int i, j;
		
		if (n > 9) {
			for (i = 1; i <= 9; i++) {
				for (j = i; j < 9; j++) {
					System.out.print(" ");
				}

				for (j = 1; j <= i; j++) {
					System.out.printf("%d", j);
				}

				for (j = i - 1; j >= 1; j--) {
					System.out.printf("%d", j);
				}
				
				System.out.println();
			}
		}
		
		else {
			for (i = 1; i <= n; i++) {
				for (j = i; j < n; j++) {
					System.out.print(" ");
				}

				for (j = 1; j <= i; j++) {
					System.out.printf("%d", j);
				}

				for (j = i - 1; j >= 1; j--) {
					System.out.printf("%d", j);
				}
				
				System.out.println();
			}
		}
	}

}
