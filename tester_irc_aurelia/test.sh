#!/bin/bash

. ./settings.sh

LOGFILE=test_output

# Initialiser ou vider le fichier de log
echo "Démarrage des connexions IRC..." > "$LOGFILE"

# Boucle de 2 itérations
for ((i=1; i<=2; i++)); do
    echo "Connexion à $SERVER sur le port $PORT (itération $i)..." | tee -a "$LOGFILE"
    
    ./ircserv $PORT $PASSWORD &
    IRC_PID=$!

    sleep 1  # Attendre que le serveur soit prêt

    TEMPFILE=$(mktemp)
    
    {
        sleep 1  # Attendre 1 seconde avant d'envoyer QUIT
        echo -e "QUIT\r\n"  # Envoyer la commande QUIT
    } > "$TEMPFILE"
    
    nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

    killall ircserv

    echo "Déconnexion (itération $i)..." | tee -a "$LOGFILE"
    
    rm "$TEMPFILE"
    sleep 1  # Attendre un peu avant la prochaine itération
done

echo "Fin des connexions IRC." | tee -a "$LOGFILE"
