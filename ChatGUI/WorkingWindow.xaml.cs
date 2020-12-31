﻿using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using ConvertedCode;
using AESEncryption;
using System.Text;
using System.Threading;

namespace ChatGUI
{
    /// <summary>
    /// Interaction logic for WorkingWindow.xaml
    /// </summary>
    public partial class WorkingWindow : Window
    {
        public WorkingWindow()
        {
            InitializeComponent();
            DataContext = new CurrentTimeViewModel();
        }

        //private readonly LoginWindow login = new LoginWindow();
        string[] pathArr = new string[10];
        string[] user_list = new string[10];
        public static volatile bool start_flag = false;
        private static volatile bool logout_flag = false;
        private static string Item;
        public static string item
        {
            set => Item = value;
            get => Item;
        }

        private void TakeUserList()
        {
            Array.Clear(user_list, 0, user_list.Length);
            LoginWindow.CPP.Check_Users_Online(LoginWindow.client, ref user_list);
            foreach (string str in user_list)
            {
                if (str.Equals(LoginWindow._friend) == false)
                    list_friend_pr.Items.Add(str);
            }  
        }

        private void SelectUser()
        {
            while (true)
            {
                if (list_friend_pr.SelectedIndex != -1)
                {
                    string _Friend = "";
                    object selected = this.list_friend_pr.SelectedItem;

                    _Friend = selected.ToString();
                    byte[] messageSent = Encoding.ASCII.GetBytes(_Friend);
                    int byteSent = LoginWindow.client.socket.Send(messageSent);
                    break;
                }

                Thread.Sleep(1500);
                TakeUserList();
            }  
        }

        public void OnOpenDialog(object sender, RoutedEventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.Multiselect = true;
            open.Filter = "All files (*.*)|*.*";
            open.FilterIndex = 1;
            open.ShowDialog();
            int index = 0;
            foreach (string str in open.FileNames)
            {
                pathArr[index] = str;
            }
        }

        #region Some Button
        public void LogoutButton_Click(object sender, RoutedEventArgs e)
        {
            LoginWindow login = new LoginWindow();
            byte[] messageSent = Encoding.ASCII.GetBytes("logout\0");
            int byteSent = LoginWindow.client.socket.Send(messageSent);
            login.Show();
            this.Close();
        }

        private void EnterPrivateChat(object sender, RoutedEventArgs e)
        {
            byte[] messageSent = Encoding.ASCII.GetBytes("private chat\0");
            int byteSent = LoginWindow.client.socket.Send(messageSent);

            if (PreChatPanel.Visibility == Visibility.Visible)
                PreChatPanel.Visibility = Visibility.Collapsed;
            PrivateChatPanel.Visibility = Visibility.Visible;
            PrivateChatInfoGrid.Visibility = Visibility.Visible;

            this.TakeUserList();
        }

        private void EnterGroupChat(object sender, RoutedEventArgs e)
        {
            if (PreChatPanel.Visibility == Visibility.Visible)
                PreChatPanel.Visibility = Visibility.Collapsed;
            GroupChatPanel.Visibility = Visibility.Visible;
            GroupChatInfoGrid.Visibility = Visibility.Visible;
        }

        private void EnterMoreOpts(object sender, RoutedEventArgs e)
        {
            if (PreChatPanel.Visibility == Visibility.Visible)
                PreChatPanel.Visibility = Visibility.Collapsed;
            MoreOptPanel.Visibility = Visibility.Visible;
        }

        public void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void ConnectPrivateChat(object sender, RoutedEventArgs e)
        {
            this.SelectUser();
            if (PrivateChatInfoGrid.Visibility == Visibility.Visible)
                PrivateChatInfoGrid.Visibility = Visibility.Collapsed;

            PrivateChatMain.Visibility = Visibility.Visible;
            LoginWindow.CPP.Start_Client_Private_Chat(LoginWindow.client);
            Thread my_thread = new Thread(new ThreadStart(AddListboxItems));
        }

        private void ReturnOptions(object sender, RoutedEventArgs e)
        {
            if (PrivateChatInfoGrid.Visibility == Visibility.Visible)
                PrivateChatInfoGrid.Visibility = Visibility.Collapsed;
            PrivateChatPanel.Visibility = Visibility.Collapsed;
            PreChatPanel.Visibility = Visibility.Visible;
        }

        private void _ReturnOptions(object sender, RoutedEventArgs e)
        {
            if (GroupChatInfoGrid.Visibility == Visibility.Visible)
                GroupChatInfoGrid.Visibility = Visibility.Collapsed;
            GroupChatPanel.Visibility = Visibility.Collapsed;
            PreChatPanel.Visibility = Visibility.Visible;
        }

        private void ConnectGroupChat(object sender, RoutedEventArgs e)
        {
            if (GroupChatInfoGrid.Visibility == Visibility.Visible)
                GroupChatInfoGrid.Visibility = Visibility.Collapsed;
            GroupChatMain.Visibility = Visibility.Visible;
        }

        private void ReturnMainOptions(object sender, RoutedEventArgs e)
        {
            if (MoreOptPanel.Visibility == Visibility.Visible)
                MoreOptPanel.Visibility = Visibility.Collapsed;
            PreChatPanel.Visibility = Visibility.Visible;
        }
        #endregion

        private void Send_Click(object sender, RoutedEventArgs e)
        {
            if (ChatBox_pr.Text.Length != 0)
            {
                PrivateChat.Items.Add("[" + DateTime.Now.ToString("HH:mm") +"] Me: " + ChatBox_pr.Text + "\n");
                PrivateChat.SelectedIndex = PrivateChat.Items.Count - 1;
                ChatBox_pr.Text = "";

                LoginWindow.CPP.Client_Send(LoginWindow.client, ChatBox_pr.Text);
            }
        }

        private void EndChat_Click(object sender, RoutedEventArgs e)
        {
            logout_flag = true;
            LoginWindow.CPP.End_Client_Private_Chat(LoginWindow.client);
        }

        public void AddListboxItems()
        {
            while (!logout_flag)
            {
                if (start_flag)
                {
                    PrivateChat.Items.Add("[" + DateTime.Now.ToString("HH:mm") + "]" + item);
                    PrivateChat.SelectedIndex = PrivateChat.Items.Count - 1;
                    start_flag = false;
                }
            }
        }
    }

    public class CurrentTimeViewModel : INotifyPropertyChanged
    {
        private string currTime;
        public CurrentTimeViewModel()
        {
            UpdateTime();
        }

        private void UpdateTime()
        {
            CurrentTime = "Login time: ";
            CurrentTime += DateTime.Now.ToString("g");
        }
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        public string CurrentTime
        {
            get { return currTime; }
            set { currTime = value; OnPropertyChanged(); }
        }
    }
}
