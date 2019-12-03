package HW_LiorTrachtman;

import java.util.Scanner;
import javax.swing.JOptionPane;
import java.awt.Dialog;

public class HW1_LiorTrachtman {
	public static void main(String args[]){
	int game = JOptionPane.YES_OPTION;
	// the loop is for the all game, it help me to check if the players want to play or not.
	while(game == 0){
		String firstPlayer = JOptionPane.showInputDialog(null,"What is your name?","First player",JOptionPane.QUESTION_MESSAGE);
		String secondPlayer = JOptionPane.showInputDialog(null,"What is your name?","Second player",JOptionPane.QUESTION_MESSAGE);
		String practice = JOptionPane.showInputDialog(null,"For practice: \n How many secret numbers do you want to show: \n Press 1,2,3 or cancel","Before we start",JOptionPane.QUESTION_MESSAGE);
		int n1 = (int)((Math.random() * (5-1+1)+1));
	    int n2 = (int)((Math.random() * (5-1+1)+1));
	    int n3 = (int)((Math.random() * (5-1+1)+1));
	// checking that the numbers of the game are different.	
	    boolean value = false;
		while (value == false)
		{
	    	if((n1 != n2) && (n2 != n3) && (n3 != n1))
	    		{value = true;}
	    	else
	    		{value = false;
	    		n1 = (int)((Math.random() * (5-1+1)+1));
	    		n2 = (int)((Math.random() * (5-1+1)+1));
	    		n3 = (int)((Math.random() * (5-1+1)+1));
	    		}
	    		//(the type of the number)((math.random() * (high-low+1)+low))
	    }
		int start = 0;
		while(start == 0)
		{
			//practice mode
			if (practice != null){
				char before = practice.charAt(0);
				switch (before){
					case '1': System.out.println(n1);
					start = 1;
					break;
					case '2': System.out.println(n1 + " " + n2); 
					start = 1;
					break;
					case '3': System.out.println(n1 + " " + n2 + " " + n3); 
					start = 1;
					break;
					default:JOptionPane.showMessageDialog(null,"You chose wrong!","Why?",JOptionPane.ERROR_MESSAGE);
					practice = JOptionPane.showInputDialog(null,"For practice: \n How many secret numbers do you want to show: \n Press 1,2,3 or cancel","Before we start",JOptionPane.QUESTION_MESSAGE);
					break;
					//this message will be the default if the char isn't 1,2 or 3 ("" not include in the checking)
		 						}
	    						  }
			else start = 1;
		}
	 	while(start == 1)
	 	//game mode
	 	{
	 		boolean win = false;
	 		while(win == false){
	 			boolean first = false;
	 			boolean second = false;
	 			while(first == false){
	 				String g11 = JOptionPane.showInputDialog(null,"Player 1# " + firstPlayer + "\n please enter your first guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 				String g12 = JOptionPane.showInputDialog(null,"Player 1# " + firstPlayer + "\n please enter your second guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 				String g13 = JOptionPane.showInputDialog(null,"Player 1# " + firstPlayer + "\n please enter your third guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 				int check1 = 0;
	 				while (check1 == 0){
	 					if(g11.equals(g12) || g12.equals(g13) || g13.equals(g11)){
	 						JOptionPane.showMessageDialog(null,"You chose same number" + "\n try again" ,"You chose wrong!",JOptionPane.ERROR_MESSAGE);
	 						g11 = JOptionPane.showInputDialog(null,"Player 1# " + firstPlayer + "\n please enter your first guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 						g12 = JOptionPane.showInputDialog(null,"Player 1# " + firstPlayer + "\n please enter your second guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 						g13 = JOptionPane.showInputDialog(null,"Player 1# " + firstPlayer + "\n please enter your third guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 						// checking if the player's numbers aren't the same.
	 															}
	 					else
	 							check1 = 1;
	 							}	 							
	 				int a11 = Integer.parseInt(g11);
	 				int a12 = Integer.parseInt(g12);
	 				int a13 = Integer.parseInt(g13);
	 				int count1 = 0;
	 					// the count will help me to check if the player chose correctly
	 					while (count1 == 0){
	 						if ((a11 == n1 || a11 == n2 || a11 == n3))
	 							count1++;
	 						if ((a12 == n1 || a12 == n2 || a12 == n3))
	 							count1++;
	 						if ((a13 == n1 || a13 == n2 || a13 == n3))
	 							count1++;
	 										}
	 					if(count1 == 3){
	 						//after the game ending the players choosing if to continue playing or not.
	 						game = JOptionPane.showConfirmDialog(null, "Congratulation!" + "\n Player " + firstPlayer + " Won!" + "\n Do you want to start a new game?","Winner!",JOptionPane.YES_NO_CANCEL_OPTION);
	 						if(game == JOptionPane.YES_OPTION){
	 							first = true;		
	 							second = true;
	 							win = true;
	 															}
	 						else{
	 							game = 1;
	 							first = true;		
	 							second = true;
	 							win = true;
	 							//ending the game
	 							}
	 									}
	 					else{
	 						JOptionPane.showMessageDialog(null,count1 + " numberes were guessed correctly","Almost",JOptionPane.INFORMATION_MESSAGE);
	 						first = true;
	 						second = false;
	 						count1 = 0;
	 						}
	 					}
	 			// the code for player two are like player one but only with different variables.
	 			while(second == false){
	 				String g21 = JOptionPane.showInputDialog(null,"Player 2# " + secondPlayer + "\n please enter your first guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 				String g22 = JOptionPane.showInputDialog(null,"Player 2# " + secondPlayer + "\n please enter your second guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 				String g23 = JOptionPane.showInputDialog(null,"Player 2# " + secondPlayer + "\n please enter your third guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 				if(g21.equals(g22) || g22.equals(g23) || g23.equals(g21)){
	 					JOptionPane.showMessageDialog(null,"You chose same number" + "\n try again" ,"You chose wrong!",JOptionPane.ERROR_MESSAGE);
	 					 g21 = JOptionPane.showInputDialog(null,"Player 2# " + secondPlayer + "\n please enter your first guess","Let's play",JOptionPane.QUESTION_MESSAGE);
		 				 g22 = JOptionPane.showInputDialog(null,"Player 2# " + secondPlayer + "\n please enter your second guess","Let's play",JOptionPane.QUESTION_MESSAGE);
		 				 g23 = JOptionPane.showInputDialog(null,"Player 2# " + secondPlayer + "\n please enter your third guess","Let's play",JOptionPane.QUESTION_MESSAGE);
	 														}
	 				else{
	 					int a21 = Integer.parseInt(g21);
	 					int a22 = Integer.parseInt(g22);
	 					int a23 = Integer.parseInt(g23);
	 					int count2 = 0;
	 					while (count2 == 0){
	 						if ((a21 == n1 || a21 == n2 || a21 == n3))
	 							count2++;
	 						if ((a22 == n1 || a22 == n2 || a22 == n3))
	 							count2++;
	 						if ((a23 == n1 || a23 == n2 || a23 == n3))
	 							count2++;
	 										}
	 					if(count2 == 3){
	 						game = JOptionPane.showConfirmDialog(null, "Congratulation!" + "\n Player " + secondPlayer + " Won!" + "\n Do you want to start a new game?","Winner!",JOptionPane.YES_NO_CANCEL_OPTION);
	 						if(game == JOptionPane.YES_OPTION){
	 							first = true;		
	 							second = true;
	 							win = true;
	 															}
	 						else{
	 							game = 1;
	 							first = true;		
	 							second = true;
	 							win = true;
	 							}
	 									}
	 					else{
	 						JOptionPane.showMessageDialog(null,count2 + " numberes were guessed correctly","Almost",JOptionPane.INFORMATION_MESSAGE);
	 						first = false;
	 						second = true;
	 						count2 = 0;
	 						}
	 					}
	 								}
	 		start = 0;
	 		//restart the game.
	 		}
	 	}
	}
	JOptionPane.showMessageDialog(null,"Thank you for playing","Game Over",JOptionPane.INFORMATION_MESSAGE);
	}
}