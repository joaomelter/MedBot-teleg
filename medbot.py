import telebot
from telebot import types
import os
from dotenv import load_dotenv

load_dotenv()
TOKEN = os.getenv('TELEGRAM_BOT_TOKEN')
bot = telebot.TeleBot(TOKEN)

# --- FUNÇÃO PARA GERAR O MENU PRINCIPAL ---
# Criamos uma função para isso para não repetir o código, já que o botão "Voltar"
# também precisa gerar este mesmo menu.
def gerar_menu_principal():
    markup = types.InlineKeyboardMarkup(row_width=2)
    ajuda = types.InlineKeyboardButton("❓ Como usar", callback_data='ajuda')
    agendar = types.InlineKeyboardButton("⏰ Agendar Remédio", callback_data='agendar')
    marcar = types.InlineKeyboardButton("📦 Gerenciar Compartimentos", callback_data='marcar')
    markup.add(ajuda, agendar, marcar)
    return markup

# --- COMANDO /start ---
# Envia a mensagem inicial com a foto e o menu principal.
@bot.message_handler(commands=['start'])
def send_welcome(message):
    try:
        # Usa a função para gerar o menu principal
        markup = gerar_menu_principal()

        # Enviando imagem com legenda e botões juntos
        with open('./img/medbot.png', 'rb') as photo_file:
            bot.send_photo(
                message.chat.id,
                photo=photo_file,
                caption=(
                    "👋 Olá! Eu sou o Medbot, seu assistente da MedBox.\n\n"
                    "Estou aqui para ajudar você a organizar seus remédios de forma inteligente. "
                    "O que você gostaria de fazer?"
                ),
                reply_markup=markup
            )
    except Exception as e:
        print(f"Erro ao enviar a imagem: {e}")
        bot.send_message(message.chat.id, "Ocorreu um erro ao iniciar. Tente novamente com /start")

# --- HANDLER CENTRAL DE CALLBACKS ---
# Este handler processa TODOS os cliques em botões inline.
@bot.callback_query_handler(func=lambda call: True)
def callback_inline(call):
    # --- BOTÃO DE VOLTAR ---
    if call.data == 'voltar_menu':
        # Gera o menu principal novamente
        markup = gerar_menu_principal()
        # Edita a legenda da foto para voltar ao texto inicial
        bot.edit_message_caption(
            chat_id=call.message.chat.id,
            message_id=call.message.message_id,
            caption=(
                "👋 Olá! Eu sou o Medbot, seu assistente da MedBox.\n\n"
                "Estou aqui para ajudar você a organizar seus remédios de forma inteligente. "
                "O que você gostaria de fazer?"
            ),
            reply_markup=markup
        )

    # --- MENU DE AJUDA ---
    elif call.data == 'ajuda':
        markup = types.InlineKeyboardMarkup()
        btn_voltar = types.InlineKeyboardButton("⬅️ Voltar", callback_data='voltar_menu')
        markup.add(btn_voltar)
        
        bot.edit_message_caption(
            chat_id=call.message.chat.id,
            message_id=call.message.message_id,
            caption=(
                "❓ *Como Usar a MedBox*\n\n"
                "• *Agendar Remédio*: Configure os horários que a MedBox deve te notificar e liberar um remédio.\n\n"
                "• *Gerenciar Compartimentos*: Associe cada remédio a um dos 7 compartimentos da caixa."
            ),
            reply_markup=markup,
            parse_mode='Markdown'
        )

    # --- MENU DE AGENDAMENTO ---
    elif call.data == 'agendar':
        markup = types.InlineKeyboardMarkup()
        btn_voltar = types.InlineKeyboardButton("⬅️ Voltar", callback_data='voltar_menu')
        markup.add(btn_voltar)
        
        bot.edit_message_caption(
            chat_id=call.message.chat.id,
            message_id=call.message.message_id,
            caption="⏰ *Agendamento de Remédios*\n\nEsta função está em desenvolvimento. Em breve você poderá configurar seus horários por aqui!",
            reply_markup=markup,
            parse_mode='Markdown'
        )

    # --- MENU DE MARCAR COMPARTIMENTO ---
    elif call.data == 'marcar':
        markup = types.InlineKeyboardMarkup(row_width=4)
        # Cria os botões de 1 a 7 dinamicamente
        botoes_comp = [types.InlineKeyboardButton(str(i), callback_data=f'comp_{i}') for i in range(1, 8)]
        btn_voltar = types.InlineKeyboardButton("⬅️ Voltar", callback_data='voltar_menu')
        
        markup.add(*botoes_comp) # Adiciona todos os botões de compartimento
        markup.add(btn_voltar)   # Adiciona o botão de voltar em uma nova linha
        
        bot.edit_message_caption(
            chat_id=call.message.chat.id,
            message_id=call.message.message_id,
            caption="📦 *Gerenciar Compartimentos*\n\nSelecione o compartimento que deseja configurar:",
            reply_markup=markup,
            parse_mode='Markdown'
        )
    
    # --- LÓGICA PARA QUANDO UM COMPARTIMENTO É ESCOLHIDO ---
    elif call.data.startswith('comp_'):
        numero_comp = call.data.split('_')[1]
        
        markup = types.InlineKeyboardMarkup()
        btn_voltar = types.InlineKeyboardButton("⬅️ Voltar", callback_data='voltar_menu')
        markup.add(btn_voltar)

        bot.edit_message_caption(
            chat_id=call.message.chat.id,
            message_id=call.message.message_id,
            caption=f"✅ Você selecionou o *Compartimento {numero_comp}*.\n\nAgora, por favor, me diga qual o nome do remédio para este compartimento.",
            reply_markup=markup,
            parse_mode='Markdown'
        )

    # Responde ao callback para o Telegram saber que o clique foi processado.
    bot.answer_callback_query(call.id)

# --- MENSAGEM DE STATUS DO BOT ---
print("Bot está rodando. Pressione Ctrl+C para parar.")
try:
    bot.polling(none_stop=True)
except Exception as e:
    print(f"Ocorreu um erro ao iniciar o bot: {e}")