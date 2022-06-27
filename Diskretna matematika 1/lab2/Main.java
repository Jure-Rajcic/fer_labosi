import java.time.LocalDateTime;
import java.time.Duration;
import java.util.Scanner;

public class Main {
    public static String pattern = "";
    public static final String TEXT_RED = "\u001B[31m";
    public static final String TEXT_RESET = "\u001B[0m";
    public static final String TEXT_YELLOW = "\u001B[33m";
    public static int a, b, n;


    public static void main(String[] args){
        Scanner sc= new Scanner(System.in);
        System.out.print("Unesite redom, odvojene razmakom, parametre n, a i b: ");
        n = sc.nextInt();
        a = sc.nextInt();
        b = sc.nextInt();
        LocalDateTime t0 = LocalDateTime.now();

        if(n <= 2) throw new RuntimeException(TEXT_YELLOW + "NEISPRAVAN UNOS PODATAKA!!! (|v(G)| = " + n + " => ciklus u takvom grafu nemoze postojat)" + TEXT_RESET);


        // 1.dio labosa
        int result1 = PohlepniAlgoritam.start();
        LocalDateTime t1 = LocalDateTime.now();
        System.out.println("Pohlepni algoritam nalazi ciklus duljine " + result1 + " (" + TEXT_YELLOW + (Duration.between(t0, t1).toSeconds())+ TEXT_RESET + "s)");
        System.out.println(pattern + "\n");



        // 2.dio labosa
        int result2 = IscrpniAlgoritam.start();
        LocalDateTime t2 = LocalDateTime.now();
        System.out.println("Iscrpni algoritam nalazi ciklus duljine " + result2 + " (" + TEXT_YELLOW + (Duration.between(t1, t2).toSeconds())+ TEXT_RESET + "s)");
        System.out.println(pattern + "\n");

        if(result1 == result2) System.out.println("Pohlepni algoritam na ovom grafu daje optimalno rješenje! "); // opcenito ako je n = 3 ili a i b = 0
        else System.out.println("Pohlepni algoritam na ovom grafu ne daje optimalno rješenje! ");
    }
}
