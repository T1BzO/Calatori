
import java.io.*
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter
import java.util.*
import java.util.regex.Pattern

data class Operator(val nume: String, val password: String) {
    fun display() {
        println("Operator: $nume, Parola: $password")
    }
}

data class Utilizator(val nume: String, val prenume: String, val email: String, val password: String, val keyword: String)

data class Tren(
    val orasPlecare: String,
    val orasDestinatie: String,
    val ziPlecare: Int,
    val lunaPlecare: Int,
    val oraPlecare: Int,
    val minutPlecare: Int,
    var locuriClasa1: Int,
    var locuriClasa2: Int
) {
    fun toCSV(): String {
        return "$orasPlecare;$orasDestinatie;$ziPlecare;$lunaPlecare;$oraPlecare;$minutPlecare;$locuriClasa1;$locuriClasa2"
    }
}

object Importer {
    fun importOperator(filename: String): Operator {
        val file = File(filename)
        if (!file.exists()) {
            throw IOException("Nu s-a putut deschide fișierul pentru import: $filename")
        }

        val line = file.readLines().firstOrNull()
        return if (line != null) {
            val parts = line.split(";")
            if (parts.size < 2) {
                throw RuntimeException("Format invalid pentru operator.")
            }
            Operator(parts[0], parts[1])
        } else {
            throw RuntimeException("Fișierul este gol sau formatul este invalid.")
        }
    }

    fun importUtilizatori(filename: String): List<Utilizator> {
        val file = File(filename)
        if (!file.exists()) {
            throw IOException("Nu s-a putut deschide fișierul pentru import: $filename")
        }
        return file.readLines().map { line ->
            val parts = line.split(";")
            if (parts.size < 5) {
                throw RuntimeException("Format invalid pentru utilizator.")
            }
            Utilizator(parts[0], parts[1], parts[2], parts[3], parts[4])
        }
    }

    fun importTrenuri(filename: String): List<Tren> {
        val file = File(filename)
        if (!file.exists()) {
            throw IOException("Nu s-a putut deschide fișierul pentru import: $filename")
        }
    
        return file.readLines().mapNotNull { line ->
            val parts = line.split(";")
            if (parts.size < 8) {
                null
            } else {
                Tren(
                    parts[0], parts[1], parts[2].toInt(), parts[3].toInt(),
                    parts[4].toInt(), parts[5].toInt(), parts[6].toInt(), parts[7].toInt()
                )
            }
        }
    }
    

    object Exporter {
        fun exportUtilizator(filename: String, utilizator: Utilizator) {
            val file = File(filename)
            file.appendText("${utilizator.nume};${utilizator.prenume};${utilizator.email};${utilizator.password};${utilizator.keyword}\n")
        }
    }
    
    object ExporterTren {
        fun exportTren(filename: String, tren: Tren) {
            val file = File(filename)
            file.appendText("${tren.orasPlecare};${tren.orasDestinatie};${tren.ziPlecare};${tren.lunaPlecare};${tren.oraPlecare};${tren.minutPlecare};${tren.locuriClasa1};${tren.locuriClasa2}\n")
        }
    }
    

fun rescrieFisierCSV(numeFisier: String, trenuri: List<Tren>) {
    val file = File(numeFisier)
    file.writeText(trenuri.joinToString("\n") { it.toCSV() })
}

fun strongPasswordChecker(password: String): Int {
    val n = password.length
    if (n < 6) return 0

    var missing = 4
    var hasLower = false
    var hasUpper = false
    var hasDigit = false
    var hasSpecial = false

    for (char in password) {
        if (char.isLowerCase()) hasLower = true
        if (char.isUpperCase()) hasUpper = true
        if (char.isDigit()) hasDigit = true
        if (!char.isLetterOrDigit()) hasSpecial = true
    }

    if (hasLower) missing--
    if (hasUpper) missing--
    if (hasDigit) missing--
    if (hasSpecial) missing--

    return when {
        n in 6..8 && missing == 0 -> 1
        n > 8 && missing == 0 -> 2
        n >= 20 -> 3
        else -> 0
    }
}

fun encryptPassword(password: String, keyword: String): String {
    val keywordLength = keyword.length
    return password.mapIndexed { index, passwordChar ->
        val keyChar = keyword[index % keywordLength]
        if (passwordChar.isLetter()) {
            val base = if (passwordChar.isUpperCase()) 'A' else 'a'
            val keyBase = if (keyChar.isUpperCase()) 'A' else 'a'
            ((passwordChar - base + (keyChar - keyBase)) % 26 + base.code).toChar()
        } else {
            passwordChar
        }
    }.joinToString("")
}

fun generateKeyword(): String {
    val chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    return (1..8).map { chars.random() }.joinToString("")
}

fun isValidEmail(email: String): Boolean {
    val emailPattern = Pattern.compile("(\\w+)(\\.{1}\\w+)*@(\\w+)(\\.\\w+)+")
    return emailPattern.matcher(email).matches()
}

fun main() {
    val now = LocalDateTime.now()
    val lunaCurenta = now.monthValue
    val ziCurenta = now.dayOfMonth
    val oraCurenta = now.hour
    val minutCurent = now.minute

    while (true) {
        try {
            val filename1 = "operator.csv"
            val op = Importer.importOperator(filename1)
            val utilizatori = Importer.importUtilizatori("utilizatori.csv")
            println("Nu poti sa dechizi fisieriul trenuri.csv")
            var trenuri = Importer.importTrenuri("trenuri.csv")
            println("Buna ziua!")

            println("Cum vrei sa te loghezi?")
            println("Admin: apasa tasta 'a'")
            println("Utilizator: apasa tasta 'u'")
            println("Pentru inregistrare apasa tasta 'r'")

            val input = readLine()
            if (input.isNullOrEmpty()) {
                println("Input invalid!")
                continue
            }
            val c = input.first()

            when (c) {
                'a' -> {
                    println("Introduceti numele de utilizator: ")
                    val nume = readLine()!!
                    println("Introduceti parola: ")
                    val parola = readLine()!!

                    if (nume == op.nume && parola == op.password) {
                        println("Logare cu succes!")
                        println("Daca doriti sa adaugati un tren apasati tasta 1, daca doriti sa stergeti o clatorie apasati tasta 2, altfel apasati tasta 0: ")
                        var adaugare = readLine()!!.toInt()

                        while (adaugare == 1) {
                            println("Introduceti orasul de plecare: ")
                            val orasPlecare = readLine()!!
                            println("Introduceti orasul de destinatie: ")
                            val orasDestinatie = readLine()!!
                            println("Introduceti ziua plecarii: ")
                            val ziPlecare = readLine()!!.toInt()
                            println("Introduceti luna plecarii: ")
                            val lunaPlecare = readLine()!!.toInt()
                            println("Introduceti ora plecarii: ")
                            val oraPlecare = readLine()!!.toInt()
                            println("Introduceti minutul plecarii: ")
                            val minutPlecare = readLine()!!.toInt()
                            println("Introduceti numarul de locuri clasa 1: ")
                            val locuriClasa1 = readLine()!!.toInt()
                            println("Introduceti numarul de locuri clasa 2: ")
                            val locuriClasa2 = readLine()!!.toInt()

                            val tren = Tren(orasPlecare, orasDestinatie, ziPlecare, lunaPlecare, oraPlecare, minutPlecare, locuriClasa1, locuriClasa2)
                            ExporterTren.exportTren("trenuri.csv", tren)
                            println("Trenul a fost inregistrat cu succes!")

                            println("Daca doriti sa adaugati un alt tren apasati tasta 1, daca doriti sa stergeti un tren apasati tasta 2, altfel apasati tasta 0: ")
                            adaugare = readLine()!!.toInt()
                        }

                        while (adaugare == 2) {
                            println("Introduceti orasul de plecare: ")
                            val orasPlecare = readLine()!!
                            println("Introduceti orasul de destinatie: ")
                            val orasDestinatie = readLine()!!
                            println("Introduceti ziua plecarii: ")
                            val ziPlecare = readLine()!!.toInt()
                            println("Introduceti luna plecarii: ")
                            val lunaPlecare = readLine()!!.toInt()
                            println("Introduceti ora plecarii: ")
                            val oraPlecare = readLine()!!.toInt()
                            println("Introduceti minutul plecarii: ")
                            val minutPlecare = readLine()!!.toInt()

                            val trenDeSters = Tren(orasPlecare, orasDestinatie, ziPlecare, lunaPlecare, oraPlecare, minutPlecare, 0, 0)

                            val trenGasit = trenuri.any { tren ->
                                tren.orasPlecare == trenDeSters.orasPlecare &&
                                        tren.orasDestinatie == trenDeSters.orasDestinatie &&
                                        tren.ziPlecare == trenDeSters.ziPlecare &&
                                        tren.lunaPlecare == trenDeSters.lunaPlecare &&
                                        tren.oraPlecare == trenDeSters.oraPlecare &&
                                        tren.minutPlecare == trenDeSters.minutPlecare
                            }

                            if (trenGasit) {
                                trenuri = trenuri.filterNot { tren ->
                                    tren.orasPlecare == trenDeSters.orasPlecare &&
                                            tren.orasDestinatie == trenDeSters.orasDestinatie &&
                                            tren.ziPlecare == trenDeSters.ziPlecare &&
                                            tren.lunaPlecare == trenDeSters.lunaPlecare &&
                                            tren.oraPlecare == trenDeSters.oraPlecare &&
                                            tren.minutPlecare == trenDeSters.minutPlecare
                                }.toMutableList()
                                println("Calatoria a fost stearsa cu succes!")
                            } else {
                                println("Trenul specificat nu a fost găsit!")
                            }

                            rescrieFisierCSV("trenuri.csv", trenuri)

                            println("Daca doriti sa adaugati un alt tren apasati tasta 1, daca doriti sa stergeti un tren apasati tasta 2, altfel apasati tasta 0: ")
                            adaugare = readLine()!!.toInt()
                        }
                    } else {
                        println("Nume de administraor sau parola gresita!")
                    }
                }

                'u' -> {
                    println("Introduceti numele de utilizator: ")
                    val nume1 = readLine()!!
                    println("Introduceti parola: ")
                    val parola1 = readLine()!!

                    val utilizator = utilizatori.find { it.nume == nume1 }
                    if (utilizator != null) {
                        val encryptedInput = encryptPassword(parola1, utilizator.keyword)
                        if (encryptedInput == utilizator.password) {
                            println("Logare cu succes!")
                            println("Bine ai venit, ${utilizator.nume} ${utilizator.prenume}!")
                            println("Daca doriti sa cautati un tren apasati tasta 1, altfel apasati tasta 0: ")
                            var varianta = readLine()!!.toInt()

                            while (varianta == 1) {
                                println("Introduceti orasul de plecare: ")
                                val orasPlecare = readLine()!!
                                println("Introduceti orasul de destinatie: ")
                                val orasDestinatie = readLine()!!
                                println("Introduceti ziua plecarii: ")
                                val ziPlecare = readLine()!!.toInt()
                                println("Introduceti luna plecarii: ")
                                val lunaPlecare = readLine()!!.toInt()
                                println("Introduceti ora plecarii: ")
                                val oraPlecare = readLine()!!.toInt()
                                println("Introduceti minutul plecarii: ")
                                val minutPlecare = readLine()!!.toInt()

                                val trenGasit = trenuri.find { tren ->
                                    tren.orasPlecare == orasPlecare &&
                                            tren.orasDestinatie == orasDestinatie &&
                                            tren.ziPlecare == ziPlecare &&
                                            tren.lunaPlecare == lunaPlecare &&
                                            tren.oraPlecare == oraPlecare &&
                                            tren.minutPlecare == minutPlecare
                                }

                                if (trenGasit != null) {
                                    println("Trenul a fost gasit!")
                                    println("Numarul de locuri clasa 1 disponibile: ${trenGasit.locuriClasa1}")
                                    println("Numarul de locuri clasa 2 disponibile: ${trenGasit.locuriClasa2}")
                                    println("La ce clasa doriti sa va rezervati locul? Clasa 1 - apasati tasta 1, Clasa 2 - apasati tasta 2: ")
                                    val clasa = readLine()!!.toInt()
                                    if (clasa == 1) {
                                        println("Cate locuri doriti sa rezervati? ")
                                        val nrLocuri = readLine()!!.toInt()
                                        if (nrLocuri > trenGasit.locuriClasa1) {
                                            println("Nu sunt suficiente locuri disponibile!")
                                        } else {
                                            trenGasit.locuriClasa1 -= nrLocuri
                                            println("Rezervare efectuata cu succes!")
                                        }
                                    } else if (clasa == 2) {
                                        println("Cate locuri doriti sa rezervati? ")
                                        val nrLocuri = readLine()!!.toInt()
                                        if (nrLocuri > trenGasit.locuriClasa2) {
                                            println("Nu sunt suficiente locuri disponibile!")
                                        } else {
                                            trenGasit.locuriClasa2 -= nrLocuri
                                            println("Rezervare efectuata cu succes!")
                                        }
                                    }
                                } else {
                                    println("Trenul nu a fost gasit!")
                                }

                                println("Daca doriti sa cautati un alt tren apasati tasta 1, altfel apasati tasta 0: ")
                                varianta = readLine()!!.toInt()
                            }
                        } else {
                            println("Nume de utilizator sau parola gresita!")
                        }
                    } else {
                        println("Nume de utilizator sau parola gresita!")
                    }
                }

                'r' -> {
                    println("Introduceti numele: ")
                    val nume2 = readLine()!!
                    println("Introduceti prenumele: ")
                    val prenume2 = readLine()!!
                    println("Introduceti emailul: ")
                    var email2 = readLine()!!

                    while (!isValidEmail(email2)) {
                        println("Email invalid!")
                        println("Introduceti un alt email: ")
                        email2 = readLine()!!
                    }

                    while (utilizatori.any { it.email == email2 }) {
                        println("Emailul este deja folosit!")
                        println("Introduceti un alt email: ")
                        email2 = readLine()!!
                    }

                    println("Introduceti parola: ")
                    var parola2 = readLine()!!
                    var ok = false

                    while (!ok) {
                        when (strongPasswordChecker(parola2)) {
                            0 -> {
                                println("Parola trebuie sa aiba cel putin 6 caractere si sa contina cel putin o litera mica, o litera mare, o cifra si un caracter special!")
                                println("Introduceti o noua parola: ")
                                parola2 = readLine()!!
                            }
                            1 -> {
                                println("Parola este ok!")
                                ok = true
                            }
                            2 -> {
                                println("Parola este puternica!")
                                ok = true
                            }
                            3 -> {
                                println("Parola este prea lunga!")
                                println("Introduceti o noua parola: ")
                                parola2 = readLine()!!
                            }
                        }
                    }

                    val keyword2 = generateKeyword()
                    val utilizator1 = Utilizator(nume2, prenume2, email2, encryptPassword(parola2, keyword2), keyword2)
                    Exporter.exportUtilizator("utilizatori.csv", utilizator1)
                    println("Utilizatorul a fost inregistrat cu succes!")
                }

                else -> println("Optiune invalida!")
            }
        } catch (e: Exception) {
            println("Eroare: ${e.message}")
        }

        println("Daca doriti sa va intoarceti la meniu apasati tasta 1, altfel apasati tasta 0: ")
        val intorcereDupaInregistrare = readLine()!!.toInt()
        if (intorcereDupaInregistrare == 0) {
            break
        }
    }
}