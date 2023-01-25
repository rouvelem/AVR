using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    // Start is called before the first frame update

    [SerializeField] Sprite [] OneToThirty;
    [SerializeField] Sprite [] ToHandred;
    [SerializeField] Sprite [] bravo;
    [SerializeField] Sprite [] recommence;
    [SerializeField] Sprite [] fond;

    [SerializeField] public SpriteRenderer spriteNumbers; 
    [SerializeField] public SpriteRenderer spriteBravoTNul;
    [SerializeField] public SpriteRenderer spriteFond;
    
    int randomNumber; 
    public int randomInt;
    private buttonUI buttonUIsuccess;
                

    void Start()
    {

        spriteFond.sprite = fond[0];
        Vector3 spriteScale = spriteNumbers.transform.localScale;
        spriteNumbers.transform.localScale = new Vector3(spriteScale.x * 0.75f, spriteScale.y * 0.75f, 1.0f);
    }

    public void Update(){
        buttonUIsuccess = (buttonUI) FindObjectOfType(typeof(buttonUI));
        if (buttonUIsuccess.success == 1)
            doBravo();
        if (buttonUIsuccess.success == 0)
            doRetry();      
    }

	public void randomImage()
	{
        buttonUIsuccess = (buttonUI) FindObjectOfType(typeof(buttonUI));
        buttonUIsuccess.success = 2;

        int i = 0;
        randomInt = 999;
        while (i < 4 && randomInt > 30){
            randomInt = Random.Range(1,100); // choisir un nombre au hasard -> ira de 1 à 100 
            i++;
        }

        if (randomInt <= 30){
            randomNumber = Random.Range(0,3); //choisir une des 3 images au hasard
            spriteNumbers.sprite = OneToThirty[(randomInt-1)*3+randomNumber];
        } else {
            spriteNumbers.sprite = ToHandred[randomInt-31];
        }
        spriteNumbers.enabled = true;  
        spriteBravoTNul.enabled = false;
	}

    void doBravo(){
        spriteBravoTNul.enabled = true;
        spriteBravoTNul.sprite = bravo[0];
        buttonUIsuccess.success = 2;
    }

    void doRetry(){
        spriteBravoTNul.enabled = true;
        spriteBravoTNul.sprite = recommence[0];
        buttonUIsuccess.success = 2;

    }

    float GetScreenToWorldWidth(){
        Vector2 topRightCorner = new Vector2(1, 1);
        Vector2 edgeVector = Camera.main.ViewportToWorldPoint(topRightCorner);            
        var width = edgeVector.x * 2;
        return width;
    }
}

