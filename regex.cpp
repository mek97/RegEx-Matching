#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <queue>
#include <list>
#include <sstream>
#include <map>
using namespace std;

int priorty(char x)
{
	if(x=='(')
		return 1;
	if(x=='+')
		return 5;
	if(x=='*')
		return 5;
	if(x=='|')
		return 2;
	if(x=='.')
		return 3;
	if(x==')')
		return 6;
	return -1;
}

string post_fix_conv(string x)
{
	vector<char> s1;
	vector<char> q1;
	string y;

	for(int i=0;i<x.size()-1;i++)
	{
		if(x[i]!='|' && x[i]!='(' && x[i]!='.' && (priorty(x[i+1])==-1 || x[i+1]=='('))
		{
			x.insert(x.begin() + i+1, '.');
		}
	}
	//cout<<x<<endl;

	for(int i=0;i<x.size();i++)
	{
		//cout<<"i = "<<i<<endl;
		if( ( 0<=(x[i]-'0') && (x[i]-'0')<10 )|| x[i]=='e' )//|| x[i]=='*' || x[i]=='+')
			q1.push_back(x[i]);
		else
		{
			if(s1.size()==0 || x[i]=='(')
			{
				s1.push_back(x[i]);
			}
			else if(x[i]==')')
			{
				while(s1.back()!='(')
				{
					q1.push_back(s1.back());
					s1.pop_back();
				}
					s1.pop_back();
			}
			else
			{
				while(priorty(s1.back())>=priorty(x[i]))
				{
					q1.push_back(s1.back());
					s1.pop_back();
					if(s1.size()==0)
						break;
				}
				s1.push_back(x[i]);
			}
		}
		//print_vec(s1);
		//print_vec(q1);
	}

	while(s1.size()!=0)
	{
		q1.push_back(s1.back());
		s1.pop_back();
	}
	while(q1.size()!=0)
	{
		y.push_back(q1.back());
		q1.pop_back();
	}
	//reverse(y.begin(),y.end());
	return y;
}

// class to manage the states and the states connecting to it as per the expression
class state
{
public:
	vector <state*> q[11];
};

state* create_new_state()
	{
		state *s = new state();
		(s->q[10]).push_back(s);
		return s;
	}

class NFA
{
public:
	string x;
	state *start;
	vector <state *> final;
};

NFA conca(NFA a,NFA b)
{
	NFA c;
				c.start=a.start;
				for(int i=0;i<a.final.size();i++)
				{
					a.final[i]->q[10].push_back(b.start);
				}
				for(int i=0;i<b.final.size();i++)
					c.final.push_back(b.final[i]);
	c.x=".";
	//cout<<"okm"<<b.x<<"!"<<a.x<<"okm"<<endl;
	c.x.append(b.x);
	c.x.append(a.x);
	return c;
}

NFA OR(NFA a,NFA b)
{
	NFA c;
				c.start=create_new_state();
				c.start->q[10].push_back(a.start);
				c.start->q[10].push_back(b.start);
				for(int i=0;i<a.final.size();i++)
					c.final.push_back(a.final[i]);
				for(int i=0;i<b.final.size();i++)
					c.final.push_back(b.final[i]);
	c.x="|";
	c.x.append(b.x);
	c.x.append(a.x);
	return c;
}

bool e_connect(state * s, state *f)
{
	if(s==f)
		return true;

	for(int i=1;i<s->q[10].size();i++)
	{
		if(e_connect(s->q[10][i],f)==true)
			return true;	
	}
	return false;
}

NFA clini(NFA a)
{
	NFA c;
				c.start=create_new_state();
				state *t=create_new_state();
				c.start->q[10].push_back(a.start);
				c.start->q[10].push_back(t);

				for(int i=0;i<a.final.size();i++)
				{
					a.final[i]->q[10].push_back(t);
					if(e_connect(a.start,a.final[i])==false)
						a.final[i]->q[10].push_back(a.start);
				}
				c.final.push_back(t);
	c.x="*";
	//cout<<"ok"<<a.x<<"ok"<<endl;
	c.x.append(a.x);
	return c;
}

NFA plus_(NFA a);

NFA create(string x)
{
	NFA n;
	vector <NFA> v;
	while(x.size()!=0)
	{
		if(priorty(x.back())==-1 || x.back()=='e')
		{
			NFA temp;
			state* t=create_new_state();
			temp.start=create_new_state();
			temp.x=x.back();

			if(x.back()!='e')
				temp.start->q[x.back()-'0'].push_back(t);
			else
				temp.start=t;

			temp.final.push_back(t);
			v.push_back(temp);
			x.pop_back();
		}
		else if(x.back()=='|' || x.back()=='.')
		{
			NFA a,b,c;
			b=v.back();
			v.pop_back();
			a=v.back();
			v.pop_back();
			if(x.back()=='|')
			{
				c=OR(a,b);
				v.push_back(c);
				x.pop_back();	
			}
			else if(x.back()=='.')
			{
				if(b.x=="e")
				{
					v.push_back(a);
					x.pop_back();
				}
				else if(a.x=="e")
				{
					v.push_back(b);
					x.pop_back();
				}
				else
				{
					c=conca(a,b);
					v.push_back(c);
					x.pop_back();
				}
			}
		}
		else
		{
			NFA a,c;
			a=v.back();
			v.pop_back();
			if(x.back()=='+')
			{
				if(a.x=="e")
				{
					v.push_back(a);
					
				}
				else
				{
					c=plus_(a);
					v.push_back(c);
					
				}
				while(x.back()=='+')
					{
						x.pop_back();
						if(x.size()==0)
							break;
					}
			}
			else if(x.back()=='*')
			{
				if(a.x=="e")
				{
					v.push_back(a);
					
				}
				else
				{	
					c=clini(a);
					v.push_back(c);
					
				}
				while(x.back()=='*')
					{
						x.pop_back();
						if(x.size()==0)
							break;
					}
			}
		}
	}
	n=v.back();
	v.pop_back();

	return n;
}


void print_vec(vector <char> c)
{
	for(int i=0;i<c.size();i++)
	{
		cout<<c[i]<<" ";
	}
	cout<<endl;
	return;
}
// function to convert a string into post-fix form

// function to check if the string can be accepted or not
bool check_matching( state* curr , string x , int idx ,vector <state*> v)
{
	//cout<<curr<<" "<<x<<" "<<idx<<" ";
	int c;

	if(x.size()>idx)
	{
		if(x[idx]=='e')
			c=10;
		else
			c=x[idx]-'0';

		for(int i=1;i<curr->q[10].size();i++)
		{
			//cout<<"LOOP 1"<<endl;
			if(check_matching(curr->q[10][i], x , idx,v)==true)
			{
				return true;
			}
		}
		for(int i=0;i<curr->q[c].size();i++)
		{
			//cout<<"LOOP 2"<<endl;
			if(check_matching(curr->q[c][i], x , idx+1,v)==true)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for(int i=0;i<v.size();i++)
		{
			if(v[i]==curr)
			{
				return true;
			}
		}
		for(int i=1;i<curr->q[10].size();i++)
		{
			//cout<<"LOOP 3"<<endl;
			if(check_matching(curr->q[10][i], x , idx,v)==true)
			{
				return true;
			}
		}
	}
	return false;
}


//main function to take input and to conver the string
int main()
{	

		
	string cmd,st1,st2,st3,st5;
	cin>>st2;
	st3=post_fix_conv(st2);
	//cout<<st3<<endl;
	NFA n=create(st3);
	//cout<<n.x<<endl;
	char str[10002];
	fgets(str,10000,stdin);
	while(fgets(str,10000,stdin)!=NULL)
	{
		st5=str;
		if(st5.back()=='\n')
			st5.pop_back();
		
		if(check_matching(n.start,st5,0,n.final)==true)
			cout<<"matched"<<endl;
		else
			cout<<"not matched"<<endl;
		
	}
	
	return 0;
}

NFA plus_(NFA a)
{
	string l;
	l.append(".");
	l.append("*");
	l.append(".");
	l.append(a.x);
	l.append(a.x);
	l.append(".");
	l.append(a.x);
	l.append(".");
	l.append(a.x);
	l.append(".");
	l.append(a.x);
	l.append(a.x);

	//cout<<"ok"<<l<<"ok"<<endl;
	NFA c=create(l);
	
	return c;
}