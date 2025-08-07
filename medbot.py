import telebot
import os
from dotenv import load_dotenv # Importa a função para carregar variáveis do .env

load_dotenv()
TOKEN = os.getenv('TELEGRAM_BOT_TOKEN')
bot = telebot.TeleBot(TOKEN)

# comando /start
@bot.message_handler(commands=["start"])
def send_welcome(message):
    bot.reply_to(message, "Estes são os comandos disponíveis:\n/start - Inicia o bot\najuda - Mostra esta mensagem\niniciar - Começa a configurar sua caixa")

@bot.message_handler(func=lambda msg: msg.text.lower() == 'ajuda')
def ajuda(msg):
    bot.reply_to(msg, 'Aqui você encontrará um manual sobre como utilizar os seus comandos.\nVocê pode escrever os comandos sem barra!')

@bot.message_handler(func=lambda msg: msg.text.lower() == 'iniciar')
def iniciar(msg):
    bot.reply_to(msg, 'Bem-vindo! O que você deseja fazer?\nAH: Agendar horário\nMC: Marcar compartimento')

@bot.message_handler(func=lambda msg: msg.text.upper() == 'AH')
def AH(msg):
    bot.reply_to(msg, 'Agendamento iniciado')

@bot.message_handler(func=lambda msg: msg.text.upper() == 'MC')
def MC(msg):
    bot.reply_to(msg, 'Muito bem, escolha o número correspondente ao compartimento desejado\n1\n2\n3\n4\n5\n6\n7')

    # --- msg pra ver se tá rodando o bot ---
print("Bot está rodando. Pressione Ctrl+C para parar.")
try:
    bot.polling(none_stop=True) # none_stop=True evita que o bot pare se ocorrerem erros
except Exception as e:
    print(f"Ocorreu um erro ao iniciar o bot: {e}")

bot.infinity_polling()

