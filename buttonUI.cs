using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using System.IO.Ports;

public class buttonUI : MonoBehaviour
{
	public SerialPort serial1;
	int count;
	public int success;
	private GameManager gameManageComp;
	//public const int InfiniteTimeout = -1;
	void Start()
	{
		serial1 = new SerialPort("/dev/ttyACM1", 9600);
		if (!serial1.IsOpen) {
			print("Opening /dev/ttyACM0 baud 9600");
            serial1.Open();
            serial1.ReadTimeout = 100;
            serial1.Handshake = Handshake.None;
            if (serial1.IsOpen) { print("Open"); }
		}
		Debug.Log("The Port was created");
		count = 0;
		success = 2;

	}

	void Update(){
		if (serial1.IsOpen) {
			print("serial not null");
			try
			{
				string data = serial1.ReadLine();
				print(data);
				if (!String.IsNullOrEmpty(data)) {
					Int32.TryParse(data, out count);
					validate();
				}
				if(String.IsNullOrEmpty(data))
					print("la chaine est nulle");
				
			}
			catch (TimeoutException e)
			{
			}
		}
		else {
		}
	}


	public void OnClick() {
		if (!serial1.IsOpen) {
			serial1.Open();
			print("Opening the port tahu");
		} else {
			serial1.WriteLine("r");
			count++;
			print(count);
		}
	}

	public void Reset(){
		count = 0;
		if(!serial1.IsOpen){
			serial1.Open();
			print("Opening the port tahu");
		} else {
			serial1.Write("r");
			print("eh oh ");
			gameManageComp = (GameManager) FindObjectOfType(typeof(GameManager));
			gameManageComp.randomInt = 0;
			gameManageComp.spriteBravoTNul.enabled = false;
			gameManageComp.spriteNumbers.enabled = false;
			success = 2;
		}
	}

	public void validate(){
		gameManageComp = (GameManager) FindObjectOfType(typeof(GameManager));
		if (gameManageComp.randomInt > 0){
			if (count == gameManageComp.randomInt){
				print("well done");
				success = 1; 
			}
			else {
				print("try again");
				success = 0; 
			}
		}
	}
}

