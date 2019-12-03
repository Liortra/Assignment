//Lior Trachtman
package HW_LiorTrachtman;
import java.util.Scanner;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
public class HW2_LiorTrachtman {
	public static void main(String[] args){
		//First program - building a multiply matrix and his transpose
		JOptionPane.showMessageDialog(null, "Please enter four numbers to create two matrices:"+"\n (First matrix[1][2], Second matrix[3][4]","Let's play",JOptionPane.INFORMATION_MESSAGE);;
		int n1 = Integer.parseInt(JOptionPane.showInputDialog(null,"Enter number 1","First matrix",JOptionPane.QUESTION_MESSAGE));
		int n2 = Integer.parseInt(JOptionPane.showInputDialog(null,"Enter number 2","First matrix",JOptionPane.QUESTION_MESSAGE));
		int n3 = Integer.parseInt(JOptionPane.showInputDialog(null,"Enter number 3","Second matrix",JOptionPane.QUESTION_MESSAGE));
		int n4 = Integer.parseInt(JOptionPane.showInputDialog(null,"Enter number 4","Second matrix",JOptionPane.QUESTION_MESSAGE));
		int matrix[][] = matrix(build2(n1,n2),build2(n3,n4));
		print2(matrix);
		JOptionPane.showMessageDialog(null, "The transpose matrix is: ","Transpose",JOptionPane.INFORMATION_MESSAGE);
		print2(transpose(matrix)); 
		
		//Second program - converting bases
		int convert1 = (int)(Math.random() * (127-1+1)+1);
		char [] hexArr = rndHex(3) ;
		int convert2 = hexToDecimal(hexArr);
		JOptionPane.showMessageDialog(null, "(Bin) "+decimalToBinary(convert1)+" = (Dec) "+convert1+"\n(Hex) "+decimalToHex(convert2)+" = (Dec) "+convert2,"Converting bases",JOptionPane.INFORMATION_MESSAGE);
		
		//Third program - Sorting array
		JOptionPane.showMessageDialog(null, "Please enter size of array: ","Let's play",JOptionPane.INFORMATION_MESSAGE);
		int a1 = Integer.parseInt(JOptionPane.showInputDialog(null,"Enter size number: ","Array",JOptionPane.QUESTION_MESSAGE)); 
		int array[] = sort(build1(a1));
		print1(array);
		int a2 = Integer.parseInt(JOptionPane.showInputDialog(null,"Enter simple number betwenn 1 to 127: ","Let's play",JOptionPane.QUESTION_MESSAGE));
		JOptionPane.showMessageDialog(null, "The number you chose "+a2+" is in place "+(search(array, a2)+1)+" at the array","Converting bases",JOptionPane.INFORMATION_MESSAGE);
		JOptionPane.showMessageDialog(null, a2 + "(Dec) = "+decimalToBinary(a2)+"(Bin) = "+decimalToHex(a2)+"(Hex)","Converting bases",JOptionPane.INFORMATION_MESSAGE);
	
	}
	//method for printing array one size
	public static void print1(int[] list){
		String m = "";
		for (int i = 0; i < list.length; i++)
		      m += (list[i] + " "); 
		JOptionPane.showMessageDialog(null,new JTextArea(m),"Your array",JOptionPane.INFORMATION_MESSAGE);
	}
	//method for printing array two size
	public static void print2(int[] [] list){
		String m = "";
		for (int i = 0; i < list.length; i++){
			for (int j = 0; j < list[i].length; j++)
		      m += (list[i] [j] + " ");
		m += "\n\n";
		}
		JOptionPane.showMessageDialog(null,new JTextArea(m),"Your matrix",JOptionPane.INFORMATION_MESSAGE);
	}
	//method for building one size array
	public static int[] build1(int x){
		int[] value = new int[x];
		for(int i = 0; i < value.length; i++){
			value[i] = (int)(Math.random() * (127-1+1)+1);
			//(the type of the number)(math.random() * (high-low+1)+low)
		}
		return value;
	}
	//method for building two size array
	public static int[][] build2(int x, int y){
		int[][] array2 = new int[x] [y];
		for (int i = 0; i < array2.length; i++){
			array2[i] = build1(y);
												}
		return array2;
	}
	//method for multiply matrix
	public static int[][] matrix(int[][] list1, int[][] list2){
		int rowsize = list1.length;
		int culomnsize = list2[0].length;
		int[][] math = new int[rowsize] [culomnsize];
		for(int i = 0; i < rowsize; i++) {
            for (int j = 0; j < culomnsize; j++) {
                for (int k = 0; k < culomnsize; k++) {
                    math[i][j] += list1[i][k] * list2[k][j];
                }
            }
        }
		return math;
	}
	//method for transpose matrix
	public static int[][] transpose(int[][] list){
		int[][] temp = new int[list[0].length][list.length];
		for (int i = 0; i < list.length; i++)
            for (int j = 0; j < list[0].length; j++)
                temp[j][i] = list[i][j];
		return temp;
	}
	//method for convert from decimal to binary
	public static int decimalToBinary(int x){
		int bin = 0;
		String check = "";
		float whole = 0;
		float part = 0;
		while(x != 0){
			whole = x/2;
			part = whole %2;
			if(part == 0)
			check = check + '0';
			else
				check = check + '1';
			x = (int)whole;
					}
		bin = Integer.parseInt(check);
		return bin;
	}
	//method for convert from binary to decimal
	public static int binaryToDecimal(String bin){
	int x = 0;
	int check = Integer.parseInt(bin);
	for(int i=0; check !=0; i++){
		x = x + (int) ((check%10) * (Math.pow(2,i)));
		check = check / 10;
								}
	return x;
	}
	//method for convert from decimal to hex
	public static String decimalToHex(int decimal){
		    String hex = "";
		    while (decimal != 0){
		      int Value = decimal % 16;
		      if (Value <= 9 && Value >= 0) 
		    	  hex = (char)(Value + '0') + hex;
		      else 
		    	  hex = (char)(Value - 10 + 'A') + hex;
		      decimal = decimal / 16;          
		      					}
		    return hex;                                  
		    }
	//method for convert from hex to decimal
	public static int hexToDecimal(char[] hex){  
		  int decimal = 0;
		  // to change to order of the array
		  for(int i = 0; i < hex.length - 1 ; i++){
			  char check = hex[i];
			  hex[i] = hex[hex.length - i - 1];
			  hex[hex.length -i -1] = check;
			  }
		  //checking the variable in the array
		  for(int i = 0; i<hex.length; i++){
			  decimal = decimal + (charToDex(hex[i]) * ((int)Math.pow(16, i)));
		  }											
		  return decimal;
	  }
	//method for building an array of hex base
	public static int charToDex(char a)
	  {
		  char[] hex={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		  for(int i=0; i<=15; i++)
			  if(a==hex[i])
				  return i;
		  return 0;
	  }
	//method for drill a random hex number
	public static char[] rndHex(int size){
		char[] arr = new char[size];
		char[] arrHex = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		for(int i=0; i<arr.length;i++){
			int x = (int)(Math.random()*16);
			arr[i] = arrHex[x];
		}
		return arr;
	}
	//method for sorting the array
	public static int[] sort(int[] list){ 
		for (int i = 1; i < list.length; i++){ 
		  int check = list[i];
	      int j;
	      for (j = i - 1; j >= 0 && list[j] > check; j--)
	    	  list[j + 1] = list[j];
	      list[j + 1] = check;
	    }
		return list;
	  }
	//method for search the index of the number in the array
	public static int search(int[] list, int key){ 
		int low = 0;
	    int high = list.length - 1;
	    while (high >= low){ 
	    	int mid = (low + high) / 2;
	    	if (key < list[mid])
	    		high = mid - 1;
	    	else if (key == list[mid])
	    		return mid;
	    	else
	    		low = mid + 1;
	    					}
	    return -low - 1; // Now high < low
	  }
}
